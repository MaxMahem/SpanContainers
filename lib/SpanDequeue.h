#pragma once
#include <functional> 
#include <optional>
#include <span>

#include "internal/SpanContainer.h"
#include "ContainerErrors.h"
#include "SpanQueue.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with Dequeue like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanDequeue : public SpanQueue<T, Extent> {
    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanQueue = SpanQueue<T, Extent>;

    using SpanContainer::span;
    using SpanContainer::count;
    using SpanQueue::read;      // front
    using SpanQueue::write;     // back

    /// @brief Gets the previous write index.
    /// @returns The previous write index.
    auto constexpr previousIndex(auto index) { return index == 0 ? capacity() - 1 : index - 1; }

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

    /// @brief Gets a refrence to the item at the back of the container.
    /// @return A refrence to the item at the back of the container.
    /// @throws EmptyContainerError if the container is empty
    [[nodiscard]] constexpr reference back()
    {
        if (pointer back = try_back()) { return *back; }
        throw EmptyContainerError();
    }

    /// @brief Gets a const refrence to the item at the back of the container.
    /// @return The item at the back of the container.
    /// @throws EmptyContainerError if the container is empty
    [[nodiscard]] constexpr const_reference back() const
    {
        if (const_pointer back = try_back()) { return *back; }
        throw EmptyContainerError();
    }

    /// @brief Gets a pointer to the item at the back of the container.
    /// @return A pointer to the item at the back of the container, or nullptr if empty.
    [[nodiscard]] constexpr pointer try_back() noexcept { return !empty() ? &span[previousIndex(write)] : nullptr; }

    /// @brief Gets a const pointer to the item at the back of the container.
    /// @return A const pointer to the item at the back of the container, or nullptr if empty.
    [[nodiscard]] constexpr const_pointer try_back() const { return !empty() ? &span[previousIndex(write)] : nullptr; }

    /// @brief Puts value at the front of the container.
    /// @param value The item to place at the front of the container.
    /// @throws FullContainerError if the stack capcity is exceeded
    void push_front(const T& item) { if (!try_push_front(item)) { throw FullContainerError(capacity()); } }

    /// @brief Tries to put value at the front of the container.
    /// @param value The item to place at the front of the container.
    /// @return true if value was placed at the front of the stack; false otherwise.
    bool try_push_front(const T& item) noexcept
    {
        if (full()) { return false; }
        ++count;
        read = previousIndex(read);
        span[read] = item;
        return true;
    }

    /// @brief Removes the item from the back of the stack.
    /// @throws EmptyContainerError if the container is empty
    void pop_back() { if (!try_pop_back()) { throw EmptyContainerError(); } }

    /// @brief Tries to remove the item from the back of the stack.
    /// @return true if an item was removed; false if the stack is empty.
    bool try_pop_back()
    {
        if (empty()) { return false; }
        --count;
        write = previousIndex(write);
        return true;
    }
};

}