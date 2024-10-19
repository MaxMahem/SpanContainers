#pragma once

#include <gtest/gtest.h>

#include "push_insert_iterator.h"

#include "../ContainerFuncs/PushStraightFuncs.h"
#include "PushTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with undirected push ability asserts
/// @tparam Container the type of the container.
template <typename Container>
class PushStraightDeathTest : public PushTestFixture<Container, push_insert_iterator<Container>>,
                              public PushStraightFuncs<Container> { };

TYPED_TEST_SUITE_P(PushStraightDeathTest);

TYPED_TEST_P(PushStraightDeathTest, WhenFullPushAssert) { ASSERT_DEATH(this->fullContainer.push(6), "Container is full"); }

REGISTER_TYPED_TEST_SUITE_P(PushStraightDeathTest, WhenFullPushAssert);

}