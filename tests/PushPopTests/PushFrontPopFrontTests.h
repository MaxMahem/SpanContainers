#pragma once

#include <iterator>
#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../BaseTests.h"
#include "../PushPopFuncs/PushFrontFuncs.h"
#include "../PushPopFuncs/PopFrontFuncs.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push front, pop front ability (dequeue)
/// @tparam Container the type of the container.
template <typename Container>
class PushFrontPopFrontTests : public PushPopTestFixture<Container>, 
                               public PushFrontFuncs<Container>,
                               public PopFrontFuncs<Container> { };

TYPED_TEST_SUITE_P(PushFrontPopFrontTests);

TYPED_TEST_P(PushFrontPopFrontTests, PopDecrementsSize)
{
    for (auto popMethod : this->PopFuncs) { this->TestPopDecrement(this->push, popMethod); }
}

TYPED_TEST_P(PushFrontPopFrontTests, PopNDecrementsSizeByN)
{
    for (auto popNMethod : this->PopNFuncs) { this->TestPopNDecrement(this->push, popNMethod); }
}

TYPED_TEST_P(PushFrontPopFrontTests, PushPopIsLIFO)
{
    for (auto& pushMethod : this->PushFuncs) {
        std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
    }
}

TYPED_TEST_P(PushFrontPopFrontTests, PushPopAfterClearIsLIFO)
{
    // partially fill then clear
    for (int value : NUMBER_FILL | std::views::take(3)) { this->emptyContainer.push_back(value); }
    this->emptyContainer.clear();

    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
}

REGISTER_TYPED_TEST_SUITE_P(PushFrontPopFrontTests, 
    PopDecrementsSize, 
    PopNDecrementsSizeByN,
    PushPopIsLIFO, 
    PushPopAfterClearIsLIFO
);

}