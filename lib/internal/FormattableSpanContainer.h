#pragma once
#include <concepts>

/// @brief Internal namespace for implementation details.
namespace SpanContainers::internal {

/// @brief Concept for assisting in formatting a SpanContainer
template<typename Container>
concept FormattableSpanContainer = requires(Container container) {
    { container.size() } noexcept -> std::same_as<typename Container::size_type>;
    { Container::capacity() } noexcept -> std::same_as<typename Container::size_type>;
};

}