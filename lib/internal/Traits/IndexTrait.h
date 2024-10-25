#pragma once

#include <span>
#include "Errors/IndexOutOfRangeError.h"

namespace SpanContainers::internal {

/// @brief Represents the ability of a container to access elements via index.
/// @tparam Derived The container type with this trait.
/// @tparam T The element type of the container.
template <typename Derived, typename T> 
struct IndexTrait
{
private:
    using reference = std::span<T>::reference;
    using pointer   = std::span<T>::pointer;
    using size_type = std::span<T>::size_type;

    [[nodiscard]] constexpr const Derived& asDerived() const noexcept { return static_cast<const Derived&>(*this); }

public:
    /// @brief Gets a reference to the element at index.
    /// @param index The index of the element to get.
    /// @return A reference to the element at index.
    /// @throws std::out_of_range If index is out of range and UseExceptions is true.
    [[nodiscard]] constexpr reference operator[](size_type index) const
    {
        if constexpr (UseExceptions) { IndexOutOfRangeError::ThrowIfOutOfRange(asDerived(), index); }
        return asDerived().unsafe_at(index);
    }

    /// @brief Gets a pointer to the element at index.
    /// @param index The index of the element to get.
    /// @return A pointer to the element at index, or nullptr if index is out of bounds.
    [[nodiscard]] constexpr pointer at(size_type index) const noexcept
    {
        return index >= asDerived().size() ? nullptr : &asDerived().unsafe_at(index);
    }
};

}