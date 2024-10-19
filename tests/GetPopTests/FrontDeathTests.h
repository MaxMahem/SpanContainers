#pragma once

#include <gtest/gtest.h>

#include "../EmptyContainerTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing asserts on Span containers with push front ability asserts
/// @tparam Container the type of the container.
template <typename Container> 
class FrontDeathTest : public EmptyContainerTestFixture<Container> { };

TYPED_TEST_SUITE_P(FrontDeathTest);

TYPED_TEST_P(FrontDeathTest, WhenEmptyGetAsserts) { ASSERT_DEATH((void) this->emptyContainer.front(), "Container is empty"); }

TYPED_TEST_P(FrontDeathTest, WhenEmptyPopAsserts) { ASSERT_DEATH(this->emptyContainer.pop_front(), "Not enough items to pop."); }

TYPED_TEST_P(FrontDeathTest, WhenEmptyPopNAsserts) { ASSERT_DEATH(this->emptyContainer.pop_front(5), "Not enough items to pop"); }

REGISTER_TYPED_TEST_SUITE_P(FrontDeathTest,
    WhenEmptyGetAsserts,
    WhenEmptyPopAsserts,
    WhenEmptyPopNAsserts
);

}