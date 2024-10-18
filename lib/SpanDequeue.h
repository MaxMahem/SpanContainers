#pragma once

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
class SpanDequeue : public SpanQueue<T, Extent>,
                    public internal::PushFrontTrait<SpanDequeue<T, Extent>, T>,
                    public internal::PopBackTrait<SpanDequeue<T, Extent>, T>
{
    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanQueue = SpanQueue<T, Extent>;

    using SpanContainer::span;
    using SpanContainer::count;
    using SpanQueue::read;      // front
    using SpanQueue::write;     // back

    /// @brief Gets the previous write index.
    /// @returns The previous write index.
    auto constexpr previousIndex(auto index) const { return index == 0 ? Extent - 1 : index - 1; }

public:
    using SpanContainer::size_type;
    using SpanContainer::reference;

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanDequeue";

    using SpanQueue::SpanQueue;
    using SpanQueue::operator=;

    using internal::PopBackTrait<SpanDequeue<T, Extent>, T>::unsafe_pop_back;

    /// @brief Gets a reference to the last item in the container without a bounds check.
    /// @return A reference to the last item in the container.
    [[nodiscard]] constexpr reference unsafe_back() const noexcept 
    {
        assert(count > 0 && "Container is empty.");
        return span[previousIndex(write)]; 
    }

    /// @brief Assign a value to the front of the container without a capacity check.
    /// @param value The item to move to the front of the container.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void unsafe_push_front(U&& value) noexcept(std::is_nothrow_assignable<T, T&&>::value)
    {
        assert(count < Extent && "Container is full.");
        ++count;
        read = previousIndex(read);
        span[read] = std::forward<U>(value);
    }

    /// @brief Removes n items from the back of the container without a bounds check.
    /// @param n the number of items to remove.
    constexpr void unsafe_pop_back(size_type n) noexcept
    {
        assert(count - n >= 0 && "Not enough items to pop.");
        count -= n;
        write = (write + Extent - n) % Extent;
    }
};

}