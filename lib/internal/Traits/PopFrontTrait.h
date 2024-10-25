#pragma once

#include <stdexcept>
#include <type_traits>

#include "SpanContainerFormatter.h"
#include "Errors/EmptyContainerError.h"
#include "Errors/InsufficentItemsError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> 
struct PopFrontTrait
{
private:
    using reference = std::span<T>::reference;
    using pointer   = std::span<T>::pointer;
    using size_type = std::span<T>::size_type;

    [[nodiscard]] constexpr Derived& asDerived() noexcept { return static_cast<Derived&>(*this); }
    [[nodiscard]] constexpr const Derived& asDerived() const noexcept { return static_cast<const Derived&>(*this); }

public:
    /// @brief Gets a refrence to the item at the front of the container.
    /// @return A refrence to the item at the front of the container.
    /// @throws EmptyContainerError if the container is empty and UseExceptions is true.
    [[nodiscard]] constexpr reference front() const noexcept(!UseExceptions)
    {
        if constexpr (UseExceptions) { EmptyContainerError::ThrowIfEmpty(asDerived()); }
        return asDerived().unsafe_front();
    }

    /// @brief Gets a pointer to the item at the front of the container.
    /// @return A pointer to the item at the front of the container or nullptr if empty.
    [[nodiscard]] constexpr pointer try_front() const noexcept
    {
        return !asDerived().empty() ? &asDerived().unsafe_front() : nullptr;
    }

    /// @brief Removes an item from the front of the container.
    /// @param The number of items to remove from the container.
    /// @throws EmptyContainerError if the container is empty and UseExceptions is true.
    constexpr void pop_front() noexcept(!UseExceptions) 
    { 
        if constexpr (UseExceptions) { EmptyContainerError::ThrowIfEmpty(asDerived()); }
        asDerived().unsafe_pop_front(1);
    }

    /// @brief Removes n items from the front of the container.
    /// @throws InsufficentItemsError if there are less than n items in the container and UseExceptions is true.
    constexpr void pop_front(size_type n) noexcept(!UseExceptions)
    {
        if constexpr (UseExceptions) { InsufficentItemsError::ThrowIfInsufficentItems(asDerived(), n); }
        asDerived().unsafe_pop_front(n);
    }

    /// @brief Tries to remove an item from the front of the container.
    /// @return true if an item was removed; false if the container is empty.
    constexpr bool try_pop_front() noexcept { return try_pop_front(1); }

    /// @brief Tries to remove n items from the front of the container.
    /// @param The number of items to remove from the container.
    /// @return true if n items were removed; false if n is greater than size.
    constexpr bool try_pop_front(size_type n) noexcept
    {
        if (n > asDerived().size()) { return false; }
        asDerived().unsafe_pop_front(n);
        return true;
    }
};

}