#pragma once

#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../ContainerFuncs/PushStraightFuncs.h"
#include "../ContainerFuncs/PopBackFuncs.h"
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

TYPED_TEST_P(PushStraightPopBackTests, PopNDecrementsSizeByN)
{
    for (auto popNMethod : this->PopNFuncs) { this->TestPopNDecrement(this->push, popNMethod); }
}

REGISTER_TYPED_TEST_SUITE_P(PushStraightPopBackTests, 
    PopDecrementsSize, 
    PopNDecrementsSizeByN
);

}