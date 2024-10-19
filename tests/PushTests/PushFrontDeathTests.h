#pragma once

#include <gtest/gtest.h>

#include "../ContainerFuncs/PushFrontFuncs.h"
#include "PushTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push front ability asserts
/// @tparam Container the type of the container.
template <typename Container>
class PushFrontDeathTest : public PushTestFixture<Container, std::front_insert_iterator<Container>>,
                           public PushFrontFuncs<Container> { };

TYPED_TEST_SUITE_P(PushFrontDeathTest);

TYPED_TEST_P(PushFrontDeathTest, WhenFullPushAssert) { ASSERT_DEATH(this->fullContainer.push_front(6), "Container is full"); }

REGISTER_TYPED_TEST_SUITE_P(PushFrontDeathTest, WhenFullPushAssert);

}