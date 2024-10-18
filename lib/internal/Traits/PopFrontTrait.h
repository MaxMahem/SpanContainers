#pragma once

#include <stdexcept>
#include <type_traits>

#include "Errors/EmptyContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> class PopFrontTrait {
    using reference = std::span<T>::reference;
    using pointer   = std::span<T>::pointer;
    using size_type = std::span<T>::size_type;

public:
    /// @brief Gets a refrence to the item at the front of the container.
    /// @return A refrence to the item at the front of the container.
    /// @throws EmptyContainerError if the container is empty
    [[nodiscard]] constexpr reference front() const
    {
        if (pointer front = try_front()) { return *front; }
        throw EmptyContainerError();
    }

    /// @brief Gets a pointer to the item at the front of the container.
    /// @return A pointer to the item at the front of the container or nullptr if empty.
    [[nodiscard]] constexpr pointer try_front() const noexcept
    {
        const auto& derived = static_cast<const Derived&>(*this);
        return !derived.empty() ? &derived.unsafe_front() : nullptr;
    }

    /// @brief Removes an item from the front of the container.
    /// @param The number of items to remove from the container.
    /// @throws EmptyContainerError if the container is empty
    constexpr void pop_front() { if (!try_pop_front(1)) { throw EmptyContainerError(); } }

    /// @brief Removes n items from the front of the container.
    /// @throws std::out_of_range if there are less than n items in the container.
    constexpr void pop_front(size_type n)
    {
        auto& derived = static_cast<Derived&>(*this);
        if (!try_pop_front(n)) {
            throw std::out_of_range(std::format("Not enough items in container '{}' to pop {} items.", derived, n));
        }
    }

    /// @brief Tries to remove an item from the front of the container.
    /// @return true if an item was removed; false if the stack is empty.
    constexpr bool try_pop_front() noexcept { return try_pop_front(1); }

    /// @brief Tries to remove n items from the front of the container.
    /// @param The number of items to remove from the container.
    /// @return true if n items were removed; false if n is greater than size.
    constexpr bool try_pop_front(size_type n) noexcept
    {
        auto& derived = static_cast<Derived&>(*this);
        if (n > derived.size()) { return false; }
        static_cast<Derived&>(*this).unsafe_pop_front(n);
        return true;
    }

    /// @brief Removes an item from the front of the container without a bounds check.
    constexpr void unsafe_pop_front() noexcept { static_cast<Derived&>(*this).unsafe_pop_front(1); }
};

}