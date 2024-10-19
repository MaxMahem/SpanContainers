#pragma once

#include <gtest/gtest.h>

#include "../EmptyContainerTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing asserts on Span containers with push back ability
/// @tparam Container the type of the container.
template <typename Container> 
class BackDeathTest : public EmptyContainerTestFixture<Container> { };

TYPED_TEST_SUITE_P(BackDeathTest);

TYPED_TEST_P(BackDeathTest, WhenEmptyGetAsserts) { ASSERT_DEATH((void) this->emptyContainer.back(), "Container is empty"); }

TYPED_TEST_P(BackDeathTest, WhenEmptyPopAsserts) { ASSERT_DEATH(this->emptyContainer.pop_back(), "Not enough items to pop."); }

TYPED_TEST_P(BackDeathTest, WhenEmptyPopNAsserts) { ASSERT_DEATH(this->emptyContainer.pop_back(5), "Not enough items to pop"); }

REGISTER_TYPED_TEST_SUITE_P(BackDeathTest,
    WhenEmptyGetAsserts,
    WhenEmptyPopAsserts,
    WhenEmptyPopNAsserts
);

}