#pragma once
#include <format>
#include <stdexcept>

#include "internal/FixedSizeContainer.h"

namespace SpanContainers {

/// @brief Represents an error for an operation that would be invalid due to a full container
class FullContainerError : public std::out_of_range
{
public:
    /// @brief Constructs a FullContainerError using message
    /// @param The message to associate with this error.
    FullContainerError(const std::string& message = "Container is full.") : std::out_of_range(message) {}

    /// @brief Throws an exception if container is full.
    /// @tparam Container A formattable container.
    /// @param container The container to check.
    template<FixedSizeContainer Container>
    static constexpr void ThrowIfFull(const Container& container) 
    {
        if (container.full()) { throw FullContainerError(std::format("Container '{}' is full.", container)); }
    }
};

}