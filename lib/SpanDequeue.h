#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string_view>
#include <type_traits>
    
#include "internal/SpanContainer.h"
#include "internal/Traits/PushFrontTrait.h"
#include "internal/Traits/PopBackTrait.h"

#include "SpanQueue.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with Dequeue like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SC_EMPTY_BASES SpanDequeue : public SpanQueue<T, Extent>,
                                   public internal::PushFrontTrait<SpanDequeue<T, Extent>, T>,
                                   public internal::PopBackTrait<SpanDequeue<T, Extent>, T>
{
    friend struct internal::IndexTrait<SpanDequeue<T, Extent>, T>;
    friend struct internal::PushFrontTrait<SpanDequeue<T, Extent>, T>;
    friend struct internal::PopBackTrait<SpanDequeue<T, Extent>, T>;

    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanContainer::span;
    using SpanContainer::count;
    using SpanContainer::size_type;
    using SpanContainer::reference;
    using SpanContainer::const_reference;

    using SpanQueue = SpanQueue<T, Extent>;
    using SpanQueue::frontIndex;
    using SpanQueue::backIndex;

    /// @brief Gets a reference to the last item in the container without a bounds check.
    /// @return A reference to the last item in the container.
    [[nodiscard]] constexpr reference UnsafeBack() noexcept 
    {
        assert(count > 0 && "Container is empty");
        return span[backIndex.previous()];
    }

    /// @brief Gets a const reference to the last item in the container without a bounds check.
    /// @return A const reference to the last item in the container.
    [[nodiscard]] constexpr const_reference UnsafeBack() const noexcept
    {
        assert(count > 0 && "Container is empty");
        return span[backIndex.previous()];
    }

    /// @brief Assign a value to the front of the container without a capacity check.
    /// @param value The item to move to the front of the container.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void UnsafePushFront(U&& value) noexcept(std::is_nothrow_assignable<T, T&&>::value)
    {
        assert(count < Extent && "Container is full");
        ++count;
        span[--frontIndex] = std::forward<U>(value);
    }

    template<std::ranges::range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void UnsafePushFrontRange(Range&& values, size_type rangeSize)
    {
        assert(count + rangeSize <= Extent && "Range is to large for span.");
        for (auto&& value : values) { UnsafePushFront(std::forward<decltype(value)>(value)); }
    }

    /// @brief Removes n items from the back of the container without a bounds check.
    /// @param n the number of items to remove.
    constexpr void UnsafePopBack(size_type n) noexcept
    {
        assert(n <= count && "Not enough items to pop");
        count -= n;
        backIndex = backIndex - n;
    }

public:
    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanDequeue";

    using SpanQueue::SpanQueue;
    using SpanQueue::operator=;
};

}