#pragma once

#include <concepts>
#include <format>
#include <stdexcept>

#include "internal/FixedSizeContainer.h"

namespace SpanContainers {

/// @brief Represents an error that occurs if a given index is out of range for a container.
class IndexOutOfRangeError : public std::out_of_range 
{
public:
    /// @brief Constructs a new IndexOutOfRangeError
    /// @param The message to associate with this error.
    IndexOutOfRangeError(const std::string& message) : std::out_of_range(message) {}

    /// @brief Throws a IndexOutOfRangeError if index is out of range for container.
    /// @tparam Container A FixedSizeContainer.
    /// @param container The container to check the range of index on.
    /// @param index The index to check.
    template <FixedSizeContainer Container>
    static void ThrowIfOutOfRange(const Container& container, typename decltype(container.size()) index)
    {
        if (index < container.size()) { return; }
        throw IndexOutOfRangeError(std::format("Index ({}) is out of range for '{}'.", index, container));
    }
};

}
