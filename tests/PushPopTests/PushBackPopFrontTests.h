#pragma once

#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "..\BaseTests.h"
#include "..\ContainerFuncs\PushBackFuncs.h"
#include "..\ContainerFuncs\PopFrontFuncs.h"
#include "PushPopTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push back, pop front ability (queue, dequeue)
/// @tparam Container the type of the container.
template <typename Container>
class PushBackPopFrontTests : public PushPopTestFixture<Container>,
                              public PushBackFuncs<Container>,
                              public PopFrontFuncs<Container> { };

TYPED_TEST_SUITE_P(PushBackPopFrontTests);

TYPED_TEST_P(PushBackPopFrontTests, PopDecrementsSize)
{
    for (auto popMethod : this->PopFuncs) { this->TestPopDecrement(this->push, popMethod); }
}

TYPED_TEST_P(PushBackPopFrontTests, PopNDecrementsSizeByN)
{
    for (auto popNMethod : this->PopNFuncs) { this->TestPopNDecrement(this->push, popNMethod); }
}

TYPED_TEST_P(PushBackPopFrontTests, PushPopIsFIFO)
{
    for (auto& pushMethod : this->PushFuncs) {
        std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, pushMethod, this->get, this->pop);
        EXPECT_TRUE(std::ranges::equal(NUMBER_FILL, values));
    }
}

TYPED_TEST_P(PushBackPopFrontTests, PushPopAfterClearIsFIFO)
{
    // partially fill then clear
    for (int value : NUMBER_FILL | std::views::take(3)) { this->push(this->emptyContainer, value); }
    this->emptyContainer.clear();

    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_TRUE(std::ranges::equal(NUMBER_FILL, values));
}

TYPED_TEST_P(PushBackPopFrontTests, PushPopWrappingIsFIFO)
{
    // partially fill then pop-empty
    for (int value : NUMBER_FILL | std::views::take(TEST_EXTENT - 1)) { this->push(this->emptyContainer, value); }
    for (int iteration = TEST_EXTENT - 1; iteration > 0; --iteration) { this->pop(this->emptyContainer); }

    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL));
}

REGISTER_TYPED_TEST_SUITE_P(PushBackPopFrontTests, 
    PopDecrementsSize, 
    PopNDecrementsSizeByN,
    PushPopIsFIFO, 
    PushPopAfterClearIsFIFO,
    PushPopWrappingIsFIFO
);

}