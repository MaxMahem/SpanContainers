#pragma once
#include <concepts>
#include <format>
#include <string>

namespace SpanContainers {

/// @brief Represents a container type that has the properties necessary for formatting
/// @tparam Container The type of the container.
template<typename Container>
concept FormattableSpanContainer = requires(Container container) {
    { Container::TYPE_NAME } -> std::convertible_to<std::string_view>;
    { container.size() } -> std::integral;
    { container.capacity() } -> std::integral;
};

}

/// @brief A formatter for span containers
/// @tparam Container The type of the container.
template <SpanContainers::FormattableSpanContainer Container>
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
        return std::format_to(ctx.out(), "{}[{}/{}]", Container::TYPE_NAME, container.size(), container.capacity());
    }
};