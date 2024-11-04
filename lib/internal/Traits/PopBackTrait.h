#pragma once

#include <stdexcept>
#include <type_traits>

#include "Errors/EmptyContainerError.h"
#include "Errors/InsufficentItemsError.h"
#include "SpanContainerFormatter.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> 
struct PopBackTrait
{
private:
    using reference       = std::span<T>::reference;
    using const_reference = std::span<T>::const_reference;
    using pointer         = std::span<T>::pointer;
    using const_pointer   = std::span<T>::const_pointer;
    using size_type       = std::span<T>::size_type;

    [[nodiscard]] constexpr Derived& AsDerived() noexcept { return static_cast<Derived&>(*this); }
    [[nodiscard]] constexpr const Derived& AsDerived() const noexcept { return static_cast<const Derived&>(*this); }

public:
    /// @brief Gets a refrence to the item at the back of the container.
    /// @return A refrence to the item at the back of the container.
    /// @throws EmptyContainerError if the container is empty and UseExceptions is true.
    [[nodiscard]] constexpr reference back() noexcept(!UseExceptions)
    {
        if constexpr (UseExceptions) { EmptyContainerError::ThrowIfEmpty(AsDerived()); }
        return AsDerived().UnsafeBack();
    }

    /// @brief Gets a const refrence to the item at the back of the container.
    /// @return A const refrence to the item at the back of the container.
    /// @throws EmptyContainerError if the container is empty and UseExceptions is true.
    [[nodiscard]] constexpr const_reference back() const noexcept(!UseExceptions)
    {
        if constexpr (UseExceptions) { EmptyContainerError::ThrowIfEmpty(AsDerived()); }
        return AsDerived().UnsafeBack();
    }

    /// @brief Gets a pointer to the item at the back of the container.
    /// @return A pointer to the item at the back of the container or nullptr if empty.
    [[nodiscard]] constexpr pointer try_back() noexcept
    {
        return !AsDerived().empty() ? &AsDerived().UnsafeBack() : nullptr;
    }

    /// @brief Gets a const pointer to the item at the back of the container.
    /// @return A pointer to the item at the back of the container or nullptr if empty.
    [[nodiscard]] constexpr const_pointer try_back() const noexcept
    {
        return !AsDerived().empty() ? &AsDerived().UnsafeBack() : nullptr;
    }

    /// @brief Removes an item from the back of the container.
    /// @param The number of items to remove from the container.
    /// @throws EmptyContainerError if the container is empty and UseExceptions is true.
    constexpr void pop_back() noexcept(!UseExceptions) 
    { 
        if constexpr (UseExceptions) { EmptyContainerError::ThrowIfEmpty(AsDerived()); }
        AsDerived().UnsafePopBack(1);
    }

    /// @brief Removes n items from the back of the container.
    /// @throws InsufficentItemsError if there are less than n items in the container and UseExceptions is true.
    constexpr void pop_back(size_type n) noexcept(!UseExceptions)
    {
        if constexpr (UseExceptions) { InsufficentItemsError::ThrowIfInsufficentItems(AsDerived(), n); }
        AsDerived().UnsafePopBack(n);
    }

    /// @brief Tries to remove an item from the back of the container.
    /// @return true if an item was removed; false if the container is empty.
    constexpr bool try_pop_back() noexcept { return try_pop_back(1); }

    /// @brief Tries to remove n items from the back of the container.
    /// @param The number of items to remove from the container.
    /// @return true if n items were removed; false if n is greater than size.
    constexpr bool try_pop_back(size_type n) noexcept
    {
        if (n > AsDerived().size()) { return false; }
        AsDerived().UnsafePopBack(n);
        return true;
    }
};

}