#pragma once
#include <array>
#include <format>
#include <forward_list>
#include <ranges>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SpanContainerFormatter.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;


/// @brief The size of the testing array
constexpr size_t TEST_EXTENT = 5;

/// @brief Base test fixture for testing empty containers.
/// @tparam Container the type of the container.
template <typename Container> 
class EmptyContainerTestFixture : public testing::Test {
    std::array<int, Container::capacity()> array{};

    static constexpr std::array<int, Container::capacity()> CreateArray(auto&& range) {
        std::array<int, Container::capacity()> result{};
        std::ranges::copy(range, result.begin());
        return result;
    };

protected:
    Container emptyContainer;

    static constexpr std::size_t CONTAINER_SIZE = Container::capacity();

    /// @brief Numbers from smallest to largest, appropriate for FIFO containers
    static constexpr std::array<int, CONTAINER_SIZE> FILL = CreateArray(std::views::iota(1, int{ Container::capacity() + 1 }));

    constexpr std::forward_list<int> FILL_LIST() { return std::forward_list( FILL.begin(), FILL.end() ); };

    static constexpr std::array<int, CONTAINER_SIZE> FIFO = FILL;
    static constexpr std::array<int, CONTAINER_SIZE> SORTED_MIN = FILL;

    /// @brief Numbers from largest to smallest, appropriate for LIFO containers
    static constexpr std::array<int, CONTAINER_SIZE> LIFO = CreateArray(FILL | std::views::reverse);
    static constexpr std::array<int, CONTAINER_SIZE> SORTED_MAX = LIFO;

    EmptyContainerTestFixture() : emptyContainer(array) { }
};

}