#pragma once
#include "BufferSelector.h"

namespace SpanContainers {

/// @brief Internal namespace for shared implementation details.
namespace internal {

/// @brief Base type for Span containers, holding common behavior.
/// @tparam T The type of the item in the container.
/// @tparam Extent The maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanContainer {
public:
    using span_type = std::span<T, Extent>;
    using element_type      = span_type::element_type;
    using value_type        = span_type::value_type;
    using size_type         = span_type::size_type;
    using pointer           = span_type::pointer;
    using const_pointer     = span_type::const_pointer;
    using reference         = span_type::reference;
    using const_reference   = span_type::const_reference;

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
    using BufferType = BufferSelector<T, Extent>::BufferType<MaxStackSize>;

    /// @brief Default constructor for empty container
    constexpr SpanContainer() noexcept requires (Extent == 0) = default;

    /// @brief Constructs a new container wrapping buffer.
    /// @param buffer The underlying buffer to wrap.
    constexpr SpanContainer(span_type buffer) noexcept : span(buffer) { }

    SpanContainer(SpanContainer&&) = delete;
    SpanContainer& operator=(SpanContainer&&) = delete;

    SpanContainer(const SpanContainer&) noexcept = default;
    SpanContainer& operator=(const SpanContainer&) noexcept = default;

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = 0; }

    /// @brief Gets the current size of the container.
    /// @return The current size of the container.
    constexpr size_type size() const noexcept { return count; }

    /// @brief Gets the capacity of the container.
    /// @return The capcity of the container.
    static consteval size_type capacity() noexcept { return Extent; }

    /// @brief Gets if the container is empty or not.
    /// @return true if the container is empty; false otherwise.
    constexpr bool empty() const noexcept { return size() == 0; }

    /// @brief Gets if the container is full or not.
    /// @return true if the container is full; false otherwise.
    constexpr bool full() const noexcept { return size() >= capacity(); }

    /// @brief Gets the underlying span.
    /// @return the underlying span.
    constexpr span_type data() const noexcept { return span; }
};

}

}