#pragma once

#include <format>
#include <stdexcept>

namespace SpanContainers::internal {

/// @brief Represents an exception type that has the properties necessary for formatting
/// @tparam Container The type of the container.
template<typename Exception>
concept FormattableException = requires(Exception container) {
    { Exception::EXCEPTION_NAME } -> std::convertible_to<std::string_view>;
    { container.what() } -> std::convertible_to<std::string>;
};

}