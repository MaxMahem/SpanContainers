#pragma once

#include <format>
#include <stdexcept>

#include "internal/FormattableSpanContainer.h"

namespace SpanContainers {

/// @brief Represents an error for an operation that would be invalid due to an empty container
class EmptyContainerError : public std::out_of_range
{
public:
    static constexpr std::string_view EXCEPTION_NAME = "EmptyContainerError";

    /// @brief constructs a EmptyContainerError using message
    /// @param The message to associate with this error.
    EmptyContainerError(const std::string& message = "Container is empty.") : std::out_of_range(message) {}

    /// @brief Constructs a new EmptyContainerError using container and format_string
    /// @tparam Container the type of the containier, must be formattable.
    /// @param container The container whose capacity was exceeded.
    /// @param format_string the format string used to generate the message using container.
    template<internal::FormattableSpanContainer Container>
    EmptyContainerError(const Container& container, const std::string& format_string = "Container '{}' is empty.")
        : std::out_of_range(std::vformat(format_string, std::make_format_args(container))) {}
};

}