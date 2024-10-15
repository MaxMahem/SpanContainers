#pragma once

#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "..\BaseTests.h"
#include "..\PushPopFuncs\PushBackFuncs.h"
#include "..\PushPopFuncs\PopFrontFuncs.h"
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
    for (int value : NUMBER_FILL | std::views::take(3)) { this->emptyContainer.push_back(value); }
    this->emptyContainer.clear();

    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_TRUE(std::ranges::equal(NUMBER_FILL, values));
}

REGISTER_TYPED_TEST_SUITE_P(PushBackPopFrontTests, PopDecrementsSize, PushPopIsFIFO, PushPopAfterClearIsFIFO);

}