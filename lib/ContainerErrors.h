#pragma once
#include <format>
#include <stdexcept>

namespace SpanContainers {

/// @brief Represents an error for an operation that would be invalid due to an empty container
class EmptyContainerError : public std::underflow_error
{
public:
    static constexpr std::string_view EXCEPTION_NAME = "EmptyContainerError";

    /// @brief constructs a EmptyContainerError using message
    /// @param The message to associate with this error.
    EmptyContainerError(const std::string& message = "Container is empty.") : std::underflow_error(message) {}
};

/// @brief Represents an error for an operation that would be invalid due to a full container
class FullContainerError : public std::overflow_error
{
public:
    static constexpr std::string_view EXCEPTION_NAME = "FullContainerError";

    /// @brief constructs a FullContainerError using message
    /// @param The message to associate with this error.
    FullContainerError(const std::string& message = "Container is full.") : std::overflow_error(message) {}

    /// @brief constructs a new FullContainerError using capacity and formatted_message
    /// @tparam Capcity_Type the type of the capacity. Must be a formattable integral.
    /// @param capacity the capacity of the container that was exceeded
    /// @param format_string the format string used to generate the message using capacity.
    template<std::integral Capacity_Type>
    FullContainerError(Capacity_Type capacity, const std::string& format_string = "Container capacity ({}) exceeded.")
        : std::overflow_error(std::vformat(format_string, std::make_format_args(capacity))) {}
};

/// @brief Represents an exception type that has the properties necessary for formatting
/// @tparam Container The type of the container.
template<typename Exception>
concept FormattableException = requires(Exception container) {
    { Exception::EXCEPTION_NAME } -> std::convertible_to<std::string_view>;
    { container.what() } -> std::convertible_to<std::string>;
};

}

/// @brief A formatter for span containers
/// @tparam Container The type of the container.
template <SpanContainers::FormattableException Exception>
struct std::formatter<Exception> : std::formatter<std::string_view>
{
    /// @brief formats a container
    /// @tparam FormatContext the FormatContext for this format
    /// @param container the container to format
    /// @param ctx the formatting context to format to
    /// @return the formatting context with the formatted container appended
    template <typename FormatContext>
    auto format(const Exception& exception, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}: {}", Exception::EXCEPTION_NAME, exception.what());
    }
};