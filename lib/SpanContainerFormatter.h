#pragma once
#include <concepts>
#include <format>
#include <string>

#include "internal/FixedSizeContainer.h"

/// @brief A formatter for span containers
/// @tparam Container The type of the container.
template <SpanContainers::FixedSizeContainer Container>
struct std::formatter<Container> : std::formatter<std::string_view>
{
    /// @brief formats a container
    /// @tparam FormatContext the FormatContext for this format
    /// @param container the container to format
    /// @param ctx the formatting context to format to
    /// @return the formatting context with the formatted container appended
    template <typename FormatContext>
    auto format(const Container& container, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}[{}/{}]", Container::TYPE_NAME, container.size(), container.max_size());
    }
};