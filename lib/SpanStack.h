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
    using SpanContainer::size_type;
    using SpanContainer::reference;

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanStack";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using internal::PopBackTrait<SpanStack<T, Extent>, T>::unsafe_pop_back;

    /// @brief Gets a reference to the last item in the container without a bounds check.
    /// @return A reference to the last item in the container.
    [[nodiscard]] constexpr reference unsafe_back() const noexcept { return span[count - 1]; }

    /// @brief Assigns value to the back of the container, without bounds checks.
    /// @tparam U The type of the value.
    /// @param value The item to assign.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void unsafe_push_back(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        span[count++] = std::forward<U>(value);
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

    /// @brief Removes n items from the back of the container without a bounds check.
    /// @param n the number of items to remove.
    constexpr void unsafe_pop_back(size_type n) noexcept { count -= n; }

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = 0; }

    /// @brief Gets the current items in the container as a span. 
    /// @return The current items in the container as a span.
    constexpr auto as_span() const noexcept { return span.first(count); }
};

}