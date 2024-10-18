#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/Traits/PushBackTrait.h"
#include "internal/Traits/PopBackTrait.h"


namespace SpanContainers::internal {

/// @brief Represents the ability of a container to access elements via index.
/// @tparam Derived The container type with this trait.
/// @tparam T The element type of the container.
template <typename Derived, typename T> class IndexTrait {
    using reference = std::span<T>::reference;
    using pointer   = std::span<T>::pointer;
    using size_type = std::span<T>::size_type;

public:
    /// @brief Gets a reference to the element at index.
    /// @param index The index of the element to get.
    /// @return A reference to the element at index.
    /// @throws std::out_of_range If index is out of range.
    [[nodiscard]] constexpr reference operator[](size_type index) const
    {
        auto derived = static_cast<const Derived&>(*this);
        if (index >= derived.size()) { throw std::out_of_range("Index out of range."); }
        return derived.unsafe_at(index);
    }

    /// @brief Gets a pointer to the element at index.
    /// @param index The index of the element to get.
    /// @return A pointer to the element at index, or nullptr if index is out of bounds.
    [[nodiscard]] constexpr pointer at(size_type index) const noexcept
    {
        auto derived = static_cast<const Derived&>(*this);
        return index >= derived.size() ? nullptr : &derived.unsafe_at(index);
    }
};

}