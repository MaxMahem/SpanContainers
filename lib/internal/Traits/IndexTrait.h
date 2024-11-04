#pragma once

#include <span>

#include "Errors/FullContainerError.h"
#include "Errors/IndexOutOfRangeError.h"
#include "Errors/InsertIndexOutOfRangeError.h"

namespace SpanContainers::internal {

/// @brief Represents the ability of a container to access elements via index.
/// @tparam Derived The container type with this trait.
/// @tparam T The element type of the container.
template <typename Derived, typename T> 
struct IndexTrait
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
    /// @brief Gets a reference to the element at index.
    /// @param index The index of the element to get.
    /// @return A reference to the element at index.
    /// @throws std::out_of_range If index is out of range and UseExceptions is true.
    [[nodiscard]] constexpr reference operator[](size_type index)
    {
        if constexpr (UseExceptions) { IndexOutOfRangeError::ThrowIfOutOfRange(AsDerived(), index); }
        return AsDerived().UnsafeAt(index);
    }

    /// @brief Gets a const reference to the element at index.
    /// @param index The index of the element to get.
    /// @return A const reference to the element at index.
    /// @throws std::out_of_range If index is out of range and UseExceptions is true.
    [[nodiscard]] constexpr const_reference operator[](size_type index) const
    {
        if constexpr (UseExceptions) { IndexOutOfRangeError::ThrowIfOutOfRange(AsDerived(), index); }
        return AsDerived().UnsafeAt(index);
    }

    /// @brief Gets a pointer to the element at index.
    /// @param index The index of the element to get.
    /// @return A pointer to the element at index, or nullptr if index is out of bounds.
    [[nodiscard]] constexpr pointer at(size_type index) noexcept
    {
        return index >= AsDerived().size() ? nullptr : &AsDerived().UnsafeAt(index);
    }

    /// @brief Gets a const pointer to the element at index.
    /// @param index The index of the element to get.
    /// @return A const pointer to the element at index, or nullptr if index is out of bounds.
    [[nodiscard]] constexpr const_pointer at(size_type index) const noexcept
    {
        return index >= AsDerived().size() ? nullptr : &AsDerived().UnsafeAt(index);
    }

    /// @brief Inserts item before index.
    /// @tparam U The type of the item, must be assignable to T.
    /// @param index The index to insert the item before.
    /// @param item The item to insert.
    /// @throws IndexOutOfRangeError if index is out of range and UseExceptions is true.
    /// @throws FullContainerError if cotainer is full and UseExceptions is true.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void insert(size_type index, U&& item) noexcept(std::is_nothrow_assignable<T&, U&&>::value && !UseExceptions)
    {
        if constexpr (UseExceptions) { InsertIndexOutOfRangeError::ThrowIfOutOfRange(AsDerived(), index); }
        if constexpr (UseExceptions) { FullContainerError::ThrowIfFull(AsDerived()); }
        AsDerived().UnsafeInsert(index, std::forward<U>(item));
    }

    /// @brief Tries to insert item before index.
    /// @tparam U The type of the item, must be assignable to T.
    /// @param index The index to insert the item before.
    /// @param item The item to insert.
    /// @return True if item was inserted, false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_insert(size_type index, U&& item) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        if (index > AsDerived().size() || AsDerived().full()) { return false; }
        AsDerived().UnsafeInsert(index, std::forward<U>(item));
        return true;
    }
};

}