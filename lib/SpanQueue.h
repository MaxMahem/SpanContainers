#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/CircularIndex.h"
#include "internal/Traits/IndexTrait.h"
#include "internal/Traits/PushBackTrait.h"
#include "internal/Traits/PopFrontTrait.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with Queue like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SC_EMPTY_BASES SpanQueue : public internal::SpanContainer<T, Extent>,
                                 public internal::IndexTrait<SpanQueue<T, Extent>, T>,
                                 public internal::PushBackTrait<SpanQueue<T, Extent>, T>,
                                 public internal::PopFrontTrait<SpanQueue<T, Extent>, T>
{
    friend struct internal::IndexTrait<SpanQueue<T, Extent>, T>;
    friend struct internal::PushBackTrait<SpanQueue<T, Extent>, T>;
    friend struct internal::PopFrontTrait<SpanQueue<T, Extent>, T>;

    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanContainer::span;
    using SpanContainer::count;
    using SpanContainer::size_type;
    using SpanContainer::reference;
    using SpanContainer::const_reference;

    /// @brief Gets a reference to the item at the front of the container without a bounds check.
    /// @return A reference to the item at the front of the container.
    [[nodiscard]] constexpr reference UnsafeFront() noexcept 
    {
        assert(count > 0 && "Container is empty");
        return span[frontIndex]; 
    }

    /// @brief Gets a const reference to the item at the front of the container without a bounds check.
    /// @return A const reference to the item at the front of the container.
    [[nodiscard]] constexpr const_reference UnsafeFront() const noexcept
    {
        assert(count > 0 && "Container is empty");
        return span[frontIndex];
    }

    /// @brief Gets a reference to the element at index, without a bounds check.
    /// @param index The index of the element to get.
    /// @return A reference to the element at index.
    [[nodiscard]] constexpr reference UnsafeAt(size_type index) noexcept
    {
        assert(index < count && "Index out of range");
        return span[frontIndex + index];
    }

    /// @brief Gets a const reference to the element at index, without a bounds check.
    /// @param index The index of the element to get.
    /// @return A const reference to the element at index.
    [[nodiscard]] constexpr const_reference UnsafeAt(size_type index) const noexcept
    {
        assert(index < count && "Index out of range");
        return span[frontIndex + index];
    }

    /// @brief Assigns value to the back of the container, without bounds checks.
    /// @tparam U The type of the value.
    /// @param value The item to assign.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void UnsafePushBack(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        assert(count < Extent && "Container is full");
        span[backIndex] = std::forward<U>(value);
        ++backIndex;
        ++count;
    }

    template<std::ranges::random_access_range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void UnsafePushBackRange(Range&& values, size_type rangeSize) 
    {
        const auto newCount = count + rangeSize;
        assert(newCount <= Extent && "Range is to large for span.");

        const auto mid = values.begin() + (Extent - backIndex);
        if constexpr (std::is_rvalue_reference<Range&&>::value) {
            std::ranges::move(values.begin(), mid, span.begin() + backIndex);
            std::ranges::move(mid, values.end(), span.begin());
        }
        else {
            std::ranges::copy(values.begin(), mid, span.begin() + backIndex);
            std::ranges::copy(mid, values.end(), span.begin());
        }
        backIndex = backIndex + rangeSize;
        count = newCount;
    }

    template<std::ranges::range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void UnsafePushBackRange(Range&& values, size_type rangeSize)
    {
        assert(count + rangeSize <= Extent && "Range is to large for span.");
        for (auto&& value : values) { UnsafePushBack(std::forward<decltype(value)>(value)); }
    }

    /// @brief Inserts item after index U.
    /// @tparam U The type of the item.
    /// @param index The index to assign the item after.
    /// @param item The item to assign.
    template <typename U> requires std::assignable_from<T&, U&&>
    void UnsafeInsert(size_type index, U&& item)
    {
        assert(index <= count  && "Index out of range");
        assert(count <  Extent && "Container is full");

        auto actualIndex = frontIndex + index;

        // Af B. C. Db .i .. -- Af B. I. C. Db .i
        // .. Af B. C. D. Eb -- Af B. I. C. D. Eb
        if (actualIndex <= backIndex) { // Move elements backward to make space
            std::shift_right(span.begin() + actualIndex, span.begin() + backIndex + 1, 1);
            ++backIndex;
            span[actualIndex] = std::forward<U>(item);
        }
        else { // Move elements forward to make space
            std::shift_left(span.begin() + frontIndex, span.begin() + actualIndex, 1);
            --frontIndex;
            span[actualIndex.previous()] = std::forward<U>(item);
        }
        ++count;
    }

    /// @brief Remove n items from the front of the container.
    constexpr void UnsafePopFront(size_type n) noexcept
    {
        assert(n <= count && "Not enough items to pop");
        count -= n;
        frontIndex = frontIndex + n;
    }

protected:
    /// @brief index of the front element
    internal::CircularIndex<Extent> frontIndex{ 0 };

    /// @brief index of the back element
    internal::CircularIndex<Extent> backIndex{ 0 };

public:
    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanQueue";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    /// @brief Rotates the buffer so that the first entry in the queue is at the beginning of the underlying span.
    /// @details This may invalidate any existing references.
    constexpr void center() noexcept 
    {
        if (frontIndex == 0) { return; }
        if (count == 0) { frontIndex = 0; return; }

        std::rotate(span.begin(), span.begin() + frontIndex, span.end());

        frontIndex = 0;
        backIndex = count == Extent ? 0 : count;
    }

    /// @brief Returns this container as a span.
    /// @details This may require centering the buffer, which mutates this container, invalidate any references.
    /// @return This container as a span.
    constexpr auto as_span() noexcept
    {
        center();
        return span.first(count);
    }

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = backIndex = frontIndex = 0; }
};

}