#pragma once

#include "ContainerErrors.h"
#include "internal/SpanContainer.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with Queue like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanQueue : public internal::SpanContainer<T, Extent> {
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
    using SpanContainer::capacity;
    using SpanContainer::size;

    /// @brief Gets a refrence to the item at the front of the container.
    /// @return A refrence to the item at the front of the container.
    /// @throws EmptyContainerError if the container is empty
    [[nodiscard]] constexpr reference front()
    {
        if (pointer front = try_front()) { return *front; }
        throw EmptyContainerError();
    }

    /// @brief Gets a const refrence to the item at the front of the container.
    /// @return The item at the front of the container.
    /// @throws EmptyContainerError if the container is empty
    [[nodiscard]] constexpr const_reference front() const
    {
        if (const_pointer front = try_front()) { return *front; }
        throw EmptyContainerError();
    }

    /// @brief Gets a pointer to the item at the front of the container.
    /// @return A pointer to the item at the front of the container, or nullptr if empty.
    [[nodiscard]] constexpr pointer try_front() noexcept { return !empty() ? &span[read] : nullptr; }

    /// @brief Gets a const pointer to the item at the front of the container.
    /// @return A const pointer to the item at the front of the container, or nullptr if empty.
    [[nodiscard]] constexpr const_pointer try_front() const { return !empty() ? &span[read] : nullptr; }

    /// @brief Puts value at the back of the container.
    /// @param value The item to place at the back of the container.
    /// @throws FullContainerError if the stack capcity is exceeded
    void push_back(const T& item) { if (!try_push_back(item)) { throw FullContainerError(capacity()); } }

    /// @brief Tries to put value at the back of the container.
    /// @param value The item to place at the back of the container.
    /// @return true if value was placed at the back of the stack; false otherwise.
    bool try_push_back(const T& item) noexcept
    {
        if (full()) { return false; }
        span[write] = item;
        write = (write + 1) % capacity();
        ++count;
        return true;
    }

    /// @brief Removes the item from the front of the stack.
    /// @throws EmptyContainerError if the container is empty
    void pop_front() { if (!try_pop_front()) { throw EmptyContainerError(); } }

    /// @brief Tries to remove the item from the front of the stack.
    /// @return true if an item was removed; false if the stack is empty.
    bool try_pop_front()
    {
        if (empty()) { return false; }
        --count;
        read = (read + 1) % capacity();
        return true;
    }
};

}