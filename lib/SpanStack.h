#pragma once
#include <span>
#include "ContainerErrors.h"
#include "internal/SpanContainer.h"

namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with stack/vector like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanStack : public internal::SpanContainer<T, Extent> {
    using SpanContainer = internal::SpanContainer<T, Extent>;

    using SpanContainer::span;
    using SpanContainer::count;

public:
    using SpanContainer::size_type;
    using SpanContainer::pointer;
    using SpanContainer::const_pointer;
    using SpanContainer::reference;
    using SpanContainer::const_reference;

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanStack";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using SpanContainer::empty;
    using SpanContainer::full;
    using SpanContainer::capacity;
    using SpanContainer::size;

    /// @brief Gets a refrence to the item at the back/top of the container.
    /// @return A refrence to the item at the back/top of the container.
    /// @throws EmptyContainerError if the container is empty
    [[nodiscard]] constexpr reference back()
    {
        if (pointer back = try_back()) { return *back; }
        throw EmptyContainerError();
    }

    /// @brief Gets a const refrence to the item at the back/top of the container.
    /// @return The item at the back/top of the container.
    /// @throws EmptyContainerError if the container is empty
    [[nodiscard]] constexpr const_reference back() const
    {
        if (const_pointer back = try_back()) { return *back; }
        throw EmptyContainerError();
    }

    /// @brief Gets a pointer to the last item in the container.
    /// @return An pointer to the last item in the container or nullptr if empty.
    [[nodiscard]] constexpr pointer try_back() noexcept { return !empty() ? &span[size() - 1] : nullptr; }

    /// @brief Gets a const pointer to the last item in the container.
    /// @return An pointer to the const last item in the container or nullptr if empty.
    [[nodiscard]] constexpr const_pointer try_back() const noexcept { return !empty() ? &span[size() - 1] : nullptr; }

    /// @brief Puts value at the back of the container.
    /// @param value The item to place at the back of the container.
    /// @throws FullContainerError if the stack capcity is exceeded
    constexpr void push_back(const_reference value)
    {
        if (!try_push_back(value)) { throw FullContainerError(capacity()); }
    }

    /// @brief Tries to put value at the back of the container.
    /// @param value The item to place at the back of the container.
    /// @return true if value was placed at the back of the stack; false otherwise.
    constexpr bool try_push_back(const_reference value) noexcept
    {
        if (full()) { return false; }
        span[count++] = value;
        return true;
    }

    /// @brief Removes the item from the back/top of the stack.
    /// @throws EmptyContainerError if the container is empty
    constexpr void pop_back() { if (!try_pop_back()) { throw EmptyContainerError(); } }

    /// @brief Tries to remove the item from the back/top of the stack.
    /// @return true if an item was removed; false if the stack is empty.
    constexpr bool try_pop_back() noexcept
    {
        if (empty()) { return false; }
        count--;
        return true;
    }

    /// @brief Gets the current items in the container as a span. 
    /// @return The current items in the container as a span.
    constexpr auto as_span() const noexcept { return span.first(size()); }
};

}