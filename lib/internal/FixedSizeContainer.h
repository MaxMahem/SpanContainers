#pragma once
#include <concepts>

namespace SpanContainers {

/// @brief Represents a container with a fixed size.
template<typename Container>
concept FixedSizeContainer = requires(Container container) {
    { container.size() }     -> std::same_as<typename Container::size_type>;
    { container.max_size() } -> std::same_as<typename Container::size_type>;
    { container.empty() }    -> std::same_as<bool>;
    { container.full() }     -> std::same_as<bool>;
};

}