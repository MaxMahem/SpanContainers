#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

#include "internal/FixedSizeContainer.h"

namespace SpanContainers {

/// @brief Represents an error for an operation that would be invalid due to an empty container
class EmptyContainerError : public std::out_of_range
{
public:
    /// @brief constructs a EmptyContainerError using message
    /// @param The message to associate with this error.
    EmptyContainerError(const std::string& message) : std::out_of_range(message) {}

    /// @brief Throws a EmptyContainerError exception if container is empty.
    /// @tparam Container A FixedSizeContainer.
    /// @param container The container to check if empty.
    template<FixedSizeContainer Container>
    static constexpr void ThrowIfEmpty(const Container& container)
    {
        if (container.empty()) { throw EmptyContainerError(std::format("Container '{}' is empty.", container)); }
    }   
};

}