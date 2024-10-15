#pragma once

#include <format>
#include <stdexcept>

#include "internal/FormattableException.h"

/// @brief A formatter for formattable exceptions
/// @tparam Exception The type of the exception.
template <SpanContainers::internal::FormattableException Exception>
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