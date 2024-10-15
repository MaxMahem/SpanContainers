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

/// @brief Test fixture for testing base behavior of Span container types.
/// @tparam Container the type of the container.
template <typename Container> class BaseTests : public testing::Test {
    std::array<int, TEST_EXTENT> array{};

protected:
    Container emptyContainer;

    BaseTests() : emptyContainer(array) { }
};

TYPED_TEST_SUITE_P(BaseTests);

TYPED_TEST_P(BaseTests, EmptyContainerIsSizeZero) { ASSERT_THAT(this->emptyContainer, ::testing::SizeIs(0)); }

TYPED_TEST_P(BaseTests, EmptyContainerIsEmpty) { ASSERT_THAT(this->emptyContainer, ::testing::IsEmpty()); }

TYPED_TEST_P(BaseTests, StdFormatProducesIntendedResults)
{
    std::string formattedValue = std::format("{}", this->emptyContainer);
    std::string matchingRegex = std::format(
        "{}\\[{}/{}\\]",
        this->emptyContainer.TYPE_NAME,
        this->emptyContainer.size(),
        this->emptyContainer.capacity()
    );
    ASSERT_THAT(formattedValue, ::testing::MatchesRegex(matchingRegex));
}

REGISTER_TYPED_TEST_SUITE_P(BaseTests, EmptyContainerIsSizeZero, EmptyContainerIsEmpty, StdFormatProducesIntendedResults);

}