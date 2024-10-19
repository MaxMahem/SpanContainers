#pragma once

#include <gtest/gtest.h>

#include "../ContainerFuncs/PushBackFuncs.h"
#include "PushTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push back ability asserts
/// @tparam Container the type of the container.
template <typename Container>
class PushBackDeathTest : public PushTestFixture<Container, std::back_insert_iterator<Container>>,
                          public PushBackFuncs<Container> { };

TYPED_TEST_SUITE_P(PushBackDeathTest);

TYPED_TEST_P(PushBackDeathTest, WhenFullPushAssert) { ASSERT_DEATH(this->fullContainer.push_back(6), "Container is full"); }

REGISTER_TYPED_TEST_SUITE_P(PushBackDeathTest, WhenFullPushAssert);

}