#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/Traits/IndexTrait.h"
#include "internal/Traits/PushBackTrait.h"
#include "internal/Traits/PopBackTrait.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with stack/vector like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SC_EMPTY_BASES SpanStack : public internal::SpanContainer<T, Extent>,
                                 public internal::IndexTrait<SpanStack<T, Extent>, T>,
                                 public internal::PushBackTrait<SpanStack<T, Extent>, T>,
                                 public internal::PopBackTrait<SpanStack<T, Extent>, T>
{
    friend struct internal::IndexTrait<SpanStack<T, Extent>, T>;
    friend struct internal::PushBackTrait<SpanStack<T, Extent>, T>;
    friend struct internal::PopBackTrait<SpanStack<T, Extent>, T>;

    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanContainer::span;
    using SpanContainer::count;
    using SpanContainer::size_type;
    using SpanContainer::reference;

    /// @brief Gets a reference to the last item in the container without a bounds check.
    /// @return A reference to the last item in the container.
    [[nodiscard]] constexpr reference unsafe_back() const noexcept 
    { 
        assert(count > 0 && "Container is empty");
        return span[count - 1]; 
    }

    /// @brief Gets a reference to the element at index, without a bounds check.
    /// @param index The index of the element to get.
    /// @return A reference to the element at index.
    [[nodiscard]] constexpr reference unsafe_at(size_type index) const noexcept
    {
        assert(index < count && "Index out of range");
        return span[count - 1 - index];
    }

    /// @brief Assigns value to the back of the container, without bounds checks.
    /// @tparam U The type of the value.
    /// @param value The item to assign.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void unsafe_push_back(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        assert(count < Extent && "Container is full");
        span[count++] = std::forward<U>(value);
    }

    /// @brief Tries to assign values to the back of the container.
    /// @tparam Range The type of the range that contains the values.
    template<std::ranges::range Range = std::initializer_list<T>> 
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void unsafe_push_back_sized_range(Range&& values, size_type rangeSize) 
    {
        const auto newCount = count + rangeSize;
        assert(newCount <= Extent && "Range is to large for span.");

        if constexpr (std::is_rvalue_reference<Range&&>::value) { std::ranges::move(values, span.begin() + count); }
        else                                                    { std::ranges::copy(values, span.begin() + count); }
        count = newCount;
    }

    /// @brief Removes n items from the back of the container without a bounds check.
    /// @param n the number of items to remove.
    constexpr void unsafe_pop_back(size_type n) noexcept 
    { 
        assert(n <= count && "Not enough items to pop");
        count -= n; 
    }

public:
    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanStack";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using value_type = T;

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = 0; }

    /// @brief Gets the current items in the container as a span. 
    /// @return The current items in the container as a span.
    constexpr auto as_span() const noexcept { return span.first(count); }
};

}