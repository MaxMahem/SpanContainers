#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/PushPopTraits/PushBackTrait.h"
#include "internal/PushPopTraits/PopFrontTrait.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with Queue like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanQueue : public internal::SpanContainer<T, Extent>,
                  public internal::PushBackTrait<SpanQueue<T, Extent>, T>,
                  public internal::PopFrontTrait<SpanQueue<T, Extent>, T>
{
    using SpanContainer = internal::SpanContainer<T, Extent>;

    using SpanContainer::span;
    using SpanContainer::count;

protected:
    /// @brief index of the read point
    SpanContainer::size_type read = 0;

    /// @brief index of the write point
    SpanContainer::size_type write = 0;

public:
    using SpanContainer::element_type;
    using SpanContainer::size_type;
    using SpanContainer::pointer;
    using SpanContainer::const_pointer;
    using SpanContainer::reference;
    using SpanContainer::const_reference;

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanQueue";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using SpanContainer::empty;
    using SpanContainer::full;

    using internal::PopFrontTrait<SpanQueue<T, Extent>, T>::try_pop_front;

    /// @brief Gets a pointer to the item at the front of the container.
    /// @return A pointer to the item at the front of the container, or nullptr if empty.
    [[nodiscard]] constexpr pointer try_front() const noexcept { return !empty() ? &span[read] : nullptr; }

    /// @brief Tries to assign value at the back of the container.
    /// @param value The item to move to the back of the container.
    /// @return true if value was placed at the back of the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push_back(U&& value) noexcept(std::is_nothrow_assignable<T, U>::value)
    {
        if (full()) { return false; }
        span[write] = std::forward<U>(value);
        write = (write + 1) % Extent;
        ++count;
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
        new (&span[write]) T(std::forward<Args>(args)...);
        write = (write + 1) % Extent;
        ++count;
        return true;
    }

    template<std::ranges::input_range Range = std::initializer_list<T>>
    constexpr bool try_push_back_range(Range&& values) requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    {
        const auto rangeSize = std::ranges::size(values);
        const auto newCount = count + rangeSize;
        if (newCount > Extent) { return false; }
        const auto spaceAtEnd = Extent - write;
        constexpr bool isRvalue = std::is_rvalue_reference<Range&&>::value;

        const auto mid = values.begin() + spaceAtEnd;

        if constexpr (isRvalue) { 
            std::ranges::move(values.begin(), mid, span.begin() + write); 
            std::ranges::move(mid, values.end(), span.begin());
        }
        else { 
            std::ranges::copy(values.begin(), mid, span.begin() + write); 
            std::ranges::copy(mid, values.end(), span.begin());
        }
        write = (write + rangeSize) % Extent;;  // Wrap around to the front
        count = newCount;
        return true;
    }

    /// @brief Tries to remove n items from the front of the container.
    /// @return true if n items were removed; false if n is greater than the current size.
    constexpr bool try_pop_front(size_type n) noexcept
    {
        if (n > count) { return false; }
        count -= n;
        read = (read + n) % Extent;
        return true;
    }

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = write = read = 0; }
};

}