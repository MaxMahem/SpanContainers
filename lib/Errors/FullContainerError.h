#pragma once
#include <format>
#include <stdexcept>

#include "internal/FormattableSpanContainer.h"

namespace SpanContainers {

/// @brief Represents an error for an operation that would be invalid due to a full container
class FullContainerError : public std::out_of_range
{
public:
    static constexpr std::string_view EXCEPTION_NAME = "FullContainerError";

    /// @brief Constructs a FullContainerError using message
    /// @param The message to associate with this error.
    FullContainerError(const std::string& message = "Container is full.") : std::out_of_range(message) {}

    /// @brief Constructs a new FullContainerError using capacity and format_string
    /// @tparam Integer the type of the capacity. Must be a formattable integral.
    /// @param Capacity the capacity of the container that was exceeded
    /// @param format_string the format string used to generate the message using capacity.
    template<std::integral Integer>
    FullContainerError(Integer capacity, const std::string& format_string = "Container capacity ({}) exceeded.")
        : std::out_of_range(std::vformat(format_string, std::make_format_args(capacity))) {}

    /// @brief Constructs a new FullContainerError using container and format_string
    /// @tparam Container the type of the containier, must be formattable.
    /// @param container The container whose capacity was exceeded.
    /// @param format_string the format string used to generate the message using container.
    template<internal::FormattableSpanContainer Container>
    FullContainerError(const Container& container, const std::string& format_string = "Container '{}' capacity exceeded.")
        : std::out_of_range(std::vformat(format_string, std::make_format_args(container))) {}
};

}