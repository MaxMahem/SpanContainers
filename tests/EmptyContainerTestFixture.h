#pragma once
#include <array>
#include <format>
#include <ranges>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SpanContainerFormatter.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief The size of the testing array
constexpr size_t TEST_EXTENT = 5;
/// @brief Numbers from smallest to largest, appropriate for FIFO containers
constexpr std::array<int, TEST_EXTENT> NUMBER_FILL{ 1, 2, 3, 4, 5 };
/// @brief Numbers from largest to smallest, appropriate for LIFO containers
constexpr std::array<int, TEST_EXTENT> NUMBER_FILL_REVERSE{ 5, 4, 3, 2, 1 };

/// @brief Base test fixture for testing empty containers.
/// @tparam Container the type of the container.
template <typename Container> 
class EmptyContainerTestFixture : public testing::Test {
    std::array<int, TEST_EXTENT> array{};

protected:
    Container emptyContainer;

    EmptyContainerTestFixture() : emptyContainer(array) { }
};

}