#pragma once

#include <cstddef>
#include <string_view>
#include <type_traits>
    
#include "internal/SpanContainer.h"
#include "internal/PushPopTraits/PushFrontTrait.h"
#include "internal/PushPopTraits/PopBackTrait.h"

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
    auto constexpr previousIndex(auto index) const { return index == 0 ? capacity() - 1 : index - 1; }

public:
    using SpanContainer::element_type;
    using SpanContainer::size_type;
    using SpanContainer::pointer;
    using SpanContainer::const_pointer;
    using SpanContainer::reference;
    using SpanContainer::const_reference;

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanDequeue";

    using SpanQueue::SpanQueue;
    using SpanQueue::operator=;

    using SpanContainer::empty;
    using SpanContainer::full;
    using SpanContainer::capacity;
    using SpanContainer::size;

    using internal::PopBackTrait<SpanDequeue<T, Extent>, T>::try_pop_back;

    /// @brief Gets a pointer to the item at the back of the container.
    /// @return A pointer to the item at the back of the container, or nullptr if empty.
    [[nodiscard]] constexpr pointer try_back() const noexcept { return !empty() ? &span[previousIndex(write)] : nullptr; }

    /// @brief Tries to assign value to the front of the container.
    /// @param value The item to move to the front of the container.
    /// @return true if value was assigned at the front of the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push_front(U&& value) noexcept(std::is_nothrow_assignable<T, T&&>::value)
    {
        if (full()) { return false; }
        ++count;
        read = previousIndex(read);
        span[read] = std::forward<U>(value);
        return true;
    }

    /// @brief Tries to constructs a new element in place at the front of the container from args.
    /// @tparam ...Args The type of the arguments
    /// @param ...args The arguments used to construct the element.
    /// @return true if the element was constructed in placed at the back of the container; false otherwise.
    template<typename... Args> requires std::is_trivially_destructible<T>::value && std::constructible_from<T, Args&&...>
    constexpr bool try_emplace_front(Args&&... args)
    {
        if (full()) { return false; }
        ++count;
        read = previousIndex(read);
        new (&span[read]) T(std::forward<Args>(args)...);
        return true;
    }

    /// @brief Tries to remove n items from the back of the container.
    /// @return true if n items were removed; false if n is greater than size.
    constexpr bool try_pop_back(size_type n) noexcept
    {
        if (n > count) { return false; }
        count -= n;
        write = (write + capacity() - n) % capacity();;
        return true;
    }
};

}