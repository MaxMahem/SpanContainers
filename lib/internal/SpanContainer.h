#pragma once
#include "BufferSelector.h"

#ifndef SPAN_CONTAINERS_USE_EXCEPTIONS
#define SPAN_CONTAINERS_USE_EXCEPTIONS true
#endif

namespace SpanContainers { constexpr bool UseExceptions = SPAN_CONTAINERS_USE_EXCEPTIONS; }

/// @brief Internal namespace for implementation details.
namespace SpanContainers::internal {

/// @brief Base type for Span containers, holding common behavior.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanContainer {
public:
    using span_type = std::span<T, Extent>;
    using element_type    = span_type::element_type;
    using value_type      = span_type::value_type;
    using size_type       = span_type::size_type;
    using pointer         = span_type::pointer;
    using const_pointer   = span_type::const_pointer;
    using reference       = span_type::reference;
    using const_reference = span_type::const_reference;

protected:
    /// @brief the underlying container.
    span_type span;

    /// @brief the number of items in the container.
    size_type count = 0;

public:
    /// @brief Defines an appropriately sized buffer to be allocated on the heap or stack.
    /// @details If the span would be larger than MaxStackSize, returns a stack allocated std::array;
    /// otherwise, returns a structure wrapping a heap allocated std::array.
    /// @tparam MaxStackSize the maximum size, in bytes, to allocate on the stack.
    template <std::size_t MaxStackSize = 256>
    using BufferType = std::conditional_t<Extent * sizeof(T) <= MaxStackSize,
        std::array<T, Extent>, internal::HeapArray<T, Extent>>;

    /// @brief Default constructor for empty container
    constexpr SpanContainer() noexcept requires (Extent == 0) = default;

    /// @brief Constructs a new container wrapping buffer.
    /// @tparam Buffer the type of the underlying buffer to use. Must be an lvalue able to construct a std::span 
    /// @param buffer The underlying buffer to wrap.
    template <typename Buffer> 
    requires std::is_lvalue_reference_v<Buffer&> && std::is_constructible_v<span_type, Buffer&>
    constexpr SpanContainer(Buffer& buffer) noexcept : span(buffer) { }

    SpanContainer(span_type&&) = delete;

    /// @brief Gets the current size of the container.
    /// @return The current size of the container.
    constexpr size_type size() const noexcept { return count; }

    /// @brief Gets the capacity of the container.
    /// @return The capcity of the container.
    static consteval size_type capacity() noexcept { return Extent; }

    /// @brief Gets if the container is empty or not.
    /// @return true if the container is empty; false otherwise.
    constexpr bool empty() const noexcept { return count <= 0; }

    /// @brief Gets if the container is full or not.
    /// @return true if the container is full; false otherwise.
    constexpr bool full() const noexcept { return count >= Extent; }

    /// @brief Gets the underlying span.
    /// @return the underlying span.
    constexpr span_type data() const noexcept { return span; }
};

}