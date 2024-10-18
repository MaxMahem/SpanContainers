#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "push_insert_iterator.h"
#include "Errors/FullContainerError.h"

#include "../ContainerFuncs/PushStraightFuncs.h"
#include "PushTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push back ability
/// @tparam Container the type of the container.
template <typename Container>
class PushStraightTests : public PushTestFixture<Container, push_insert_iterator<Container>>,
                          public PushStraightFuncs<Container>{ };

TYPED_TEST_SUITE_P(PushStraightTests);

TYPED_TEST_P(PushStraightTests, WhenNotFullTryPushReturnsTrue)
{
    ASSERT_FALSE(this->emptyContainer.full());
    ASSERT_TRUE(this->emptyContainer.try_push(1));
}

TYPED_TEST_P(PushStraightTests, WhenFullClearEmptiesContainer)
{
    this->fullContainer.clear();
    ASSERT_THAT(this->fullContainer, ::testing::IsEmpty());
}

TYPED_TEST_P(PushStraightTests, WhenFullPushThrows) { ASSERT_THROW(this->fullContainer.push(6), FullContainerError); }

TYPED_TEST_P(PushStraightTests, WhenFullTryPushReturnsFalse) { ASSERT_FALSE(this->fullContainer.try_push(6)); }

TYPED_TEST_P(PushStraightTests, PushIncrementsSize)
{
    for (auto pushMethod : this->PushFuncs) { this->TestPushIncrement(this->emptyContainer, pushMethod); }
}

REGISTER_TYPED_TEST_SUITE_P(PushStraightTests,
    WhenNotFullTryPushReturnsTrue,
    WhenFullClearEmptiesContainer,
    WhenFullPushThrows,
    WhenFullTryPushReturnsFalse,
    PushIncrementsSize
);

}