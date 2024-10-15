#pragma once

#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../PushPopFuncs/PushStraightFuncs.h"
#include "../PushPopFuncs/PopBackFuncs.h"
#include "PushPopTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push back, pop back ability (stack, dequeue)
/// @tparam Container the type of the container.
template <typename Container>
class PushStraightPopBackTests : public PushPopTestFixture<Container>, 
                         public PopBackFuncs<Container>, 
                         public PushStraightFuncs<Container> { };

TYPED_TEST_SUITE_P(PushStraightPopBackTests);

TYPED_TEST_P(PushStraightPopBackTests, PopDecrementsSize)
{
    for (auto popMethod : this->PopFuncs) { this->TestPopDecrement(this->push, popMethod); }
}

TYPED_TEST_P(PushStraightPopBackTests, PushPopIsLIFO)
{
    for (auto& pushMethod : this->PushFuncs) {
        std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, pushMethod, this->get, this->pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
    }
}

TYPED_TEST_P(PushStraightPopBackTests, PushPopAfterClearIsLIFO)
{
    // partially fill then clear
    for (int value : NUMBER_FILL | std::views::take(3)) { this->emptyContainer.push(value); }
    this->emptyContainer.clear();

    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
}

REGISTER_TYPED_TEST_SUITE_P(PushStraightPopBackTests, PopDecrementsSize, PushPopIsLIFO, PushPopAfterClearIsLIFO);

}