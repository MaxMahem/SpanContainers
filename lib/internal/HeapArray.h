#pragma once
#include <array>
#include <memory>

/// @brief Internal namespace for implementation details.
namespace SpanContainers::internal {

/// @brief helper struct for creating heap-allocated std::arrays.
/// @tparam T The type of the element in the buffer.
/// @tparam Extent The number of elements in the type.
template <typename T, std::size_t Extent> struct HeapArray {
    /// @brief unique ptr to heap allocated std::array.
    std::unique_ptr<std::array<T, Extent>> ptr = std::make_unique<std::array<T, Extent>>();

    /// @brief converts this HeapArray into a std::span
    /// @note in C++23 this will work in constexpr
    [[nodiscard]] constexpr operator std::span<T, Extent>() const { return std::span(*ptr); }
};

}