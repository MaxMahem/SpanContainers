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
        const auto& derived = static_cast<const Derived&>(*this);
        if (pointer front = derived.try_front()) { return *front; }
        throw EmptyContainerError();
    }

    /// @brief Removes the item from the front of the container.
    /// @throws EmptyContainerError if the container is empty
    constexpr void pop_front() { if (!static_cast<Derived&>(*this).try_pop_front()) { throw EmptyContainerError(); } }

    /// @brief Tries to remove an item from the front of the container.
    /// @return true if an item was removed; false if the stack is empty.
    constexpr bool try_pop_front() noexcept { return static_cast<Derived&>(*this).try_pop_front(1); }

    /// @brief Removes n items from the front of the container.
    /// @throws std::out_of_range if there are less than n items in the container.
    constexpr void pop_front(size_type n)
    {
        auto& derived = static_cast<Derived&>(*this);
        if (!derived.try_pop_front(n)) {
            throw std::out_of_range(std::format("Not enough items in container '{}' to pop {} items.", derived, n));
        }
    }
};

}