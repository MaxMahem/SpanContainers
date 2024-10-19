#pragma once
#include <array>

#include "HeapArray.h"

/// @brief Internal namespace for implementation details.
namespace SpanContainers::internal {

/// @brief helper struct for selecting buffer type.
/// @tparam T The type of the element in the buffer.
/// @tparam Extent The number of elements in the type.
template <typename T, std::size_t Extent>
struct BufferSelector {
    /// Type alais for selecting a buffer type.
    /// @tparam MaxStackSize The maximum size in bytes to allocate on the stack
    template <std::size_t MaxStackSize = 256>
    using BufferType = std::conditional_t<Extent * sizeof(T) <= MaxStackSize,
        std::array<T, Extent>, internal::HeapArray<T, Extent>>;
};

}