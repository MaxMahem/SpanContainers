#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/PushPopTraits/PushBackTrait.h"
#include "internal/PushPopTraits/PopBackTrait.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with stack/vector like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanStack : public internal::SpanContainer<T, Extent>,
                  public internal::PushBackTrait<SpanStack<T, Extent>, T>,
                  public internal::PopBackTrait<SpanStack<T, Extent>, T>
{
    using SpanContainer = internal::SpanContainer<T, Extent>;

    using SpanContainer::span;
    using SpanContainer::count;

public:
    using SpanContainer::element_type;
    using SpanContainer::size_type;
    using SpanContainer::pointer;
    using SpanContainer::const_pointer;
    using SpanContainer::reference;
    using SpanContainer::const_reference;

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanStack";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using SpanContainer::empty;
    using SpanContainer::full;

    using internal::PopBackTrait<SpanStack<T, Extent>, T>::try_pop_back;

    /// @brief Gets a pointer to the last item in the container.
    /// @return An pointer to the last item in the container or nullptr if empty.
    [[nodiscard]] constexpr pointer try_back() const noexcept { return !empty() ? &span[count - 1] : nullptr; }

    /// @brief Tries to assign value at the back of the container.
    /// @param value The item to move to the back of the container.
    /// @return true if value was placed at the back of the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push_back(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        if (full()) { return false; }
        span[count++] = std::forward<U>(value);
        return true;
    }

    /// @brief Tries to constructs a new element in place at the back of the container from args.
    /// @tparam ...Args The type of the arguments
    /// @param ...args The arguments used to construct the element.
    /// @return true if the element was constructed in placed at the back of the container; false otherwise.
    template<typename... Args> requires std::is_trivially_destructible<T>::value && std::constructible_from<T, Args&&...>
    constexpr bool try_emplace_back(Args&&... args) 
    {
        if (full()) { return false; }
        new (&span[count++]) T(std::forward<Args>(args)...);
        return true;
    }

    /// @brief Tries to assign the values to the back of the container.
    /// @tparam Range The type of the range that contains the values.
    /// @return true if value was placed at the back of the container; false if not enough room for all values.
    template<std::ranges::input_range Range = std::initializer_list<T>>
    constexpr bool try_push_back_range(Range&& values) requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    {
        const auto newCount = count + std::ranges::size(values);
        if (newCount > Extent) { return false; }
        constexpr bool isRvalue = std::is_rvalue_reference<Range&&>::value;

        if constexpr (isRvalue) { std::ranges::move(values, span.begin() + count); }
        else                    { std::ranges::copy(values, span.begin() + count); }
        count = newCount;
            
        return true;
    }

    /// @brief Tries to remove n items from the back of the container.
    /// @return true if n items were removed; false if n is greater than size.
    constexpr bool try_pop_back(size_type n) noexcept
    {
        if (n > count) { return false; }
        count -= n;
        return true;
    }

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = 0; }

    /// @brief Gets the current items in the container as a span. 
    /// @return The current items in the container as a span.
    constexpr auto as_span() const noexcept { return span.first(count); }
};

}