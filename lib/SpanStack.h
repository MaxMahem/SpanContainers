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
    using SpanContainer::const_reference;

    /// @brief Gets a reference to the last item in the container without a bounds check.
    /// @return A reference to the last item in the container.
    [[nodiscard]] constexpr reference UnsafeBack() noexcept 
    { 
        assert(count > 0 && "Container is empty");
        return span[count - 1]; 
    }

    /// @brief Gets a const reference to the last item in the container without a bounds check.
    /// @return A const reference to the last item in the container.
    [[nodiscard]] constexpr const_reference UnsafeBack() const noexcept
    {
        assert(count > 0 && "Container is empty");
        return span[count - 1];
    }

    /// @brief Gets a reference to the element at index, without a bounds check.
    /// @param index The index of the element to get.
    /// @return A reference to the element at index.
    [[nodiscard]] constexpr reference UnsafeAt(size_type index) noexcept
    {
        assert(index < count && "Index out of range");
        return span[index];
    }

    /// @brief Gets a const reference to the element at index, without a bounds check.
    /// @param index The index of the element to get.
    /// @return A const reference to the element at index.
    [[nodiscard]] constexpr const_reference UnsafeAt(size_type index) const noexcept
    {
        assert(index < count && "Index out of range");
        return span[index];
    }

    /// @brief Assigns item to the back of the container, without bounds checks.
    /// @tparam U The type of the item.
    /// @param item The item to assign.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void UnsafePushBack(U&& item) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        assert(count < Extent && "Container is full");
        span[count++] = std::forward<U>(item);
    }

    /// @brief Tries to assign items to the back of the container.
    /// @tparam Range The type of the range that contains the items.
    template<std::ranges::range Range = std::initializer_list<T>> 
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void UnsafePushBackRange(Range&& items, size_type rangeSize) 
    {
        const auto newCount = count + rangeSize;
        assert(newCount <= Extent && "Range is to large for span.");

        if constexpr (std::is_rvalue_reference<Range&&>::value) { std::ranges::move(items, span.begin() + count); }
        else                                                    { std::ranges::copy(items, span.begin() + count); }
        count = newCount;
    }

    /// @brief Inserts item before index U.
    /// @tparam U The type of the item.
    /// @param index The index to assign the item before.
    /// @param item The item to assign.
    template <typename U> requires std::assignable_from<T&, U&&>
    void UnsafeInsert(size_type index, U&& item)
    {
        assert(index <= count && "Index out of range");
        assert(count < Extent && "Container is full");

        std::shift_right(span.begin() + index, span.begin() + count + 1, 1);
        span[index] = std::forward<U>(item);
        count++;
    }

    /// @brief Removes n items from the back of the container without a bounds check.
    /// @param n the number of items to remove.
    constexpr void UnsafePopBack(size_type n) noexcept 
    { 
        assert(n <= count && "Not enough items to pop");
        count -= n; 
    }

public:
    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanStack";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using item_type = T;

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = 0; }

    /// @brief Gets the current items in the container as a const span. 
    /// @return The current items in the container as a const span.
    constexpr auto as_span() const noexcept { return span.first(count); }

    /// @brief Gets the current items in the container as a span. 
    /// @return The current items in the container as a span.
    constexpr const auto as_span() noexcept { return span.first(count); }
};

}