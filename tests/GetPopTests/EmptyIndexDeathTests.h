#pragma once

#include <gtest/gtest.h>

#include "../EmptyContainerTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with index trait assertions
/// @tparam Container the type of the container.
template <typename Container> 
class EmptyIndexDeathTest : public EmptyContainerTestFixture<Container> { };

TYPED_TEST_SUITE_P(EmptyIndexDeathTest);

TYPED_TEST_P(EmptyIndexDeathTest, WhenEmptySubscriptAsserts) { ASSERT_DEATH((void) this->emptyContainer[0], "Index out of range"); }

REGISTER_TYPED_TEST_SUITE_P(EmptyIndexDeathTest, WhenEmptySubscriptAsserts);

}