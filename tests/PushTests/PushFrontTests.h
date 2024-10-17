#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Errors/FullContainerError.h"

#include "../BaseTests.h"
#include "../PushPopFuncs/PushFrontFuncs.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push front ability
/// @tparam Container the type of the container.
template <typename Container>
class PushFrontTests : public PushTestFixture<Container, std::front_insert_iterator<Container>>, public PushFrontFuncs<Container> { };

TYPED_TEST_SUITE_P(PushFrontTests);

TYPED_TEST_P(PushFrontTests, WhenNotFullTryPushReturnsTrue)
{
    ASSERT_FALSE(this->emptyContainer.full());
    ASSERT_TRUE(this->emptyContainer.try_push_front(1));
}

TYPED_TEST_P(PushFrontTests, WhenFullClearEmptiesContainer)
{
    this->fullContainer.clear();
    ASSERT_THAT(this->fullContainer, ::testing::IsEmpty());
}

TYPED_TEST_P(PushFrontTests, WhenFullPushFrontThrows)
{
    ASSERT_THROW(this->fullContainer.push_front(6),     FullContainerError);
}

TYPED_TEST_P(PushFrontTests, WhenFullTryPushFrontReturnsFalse)
{
    ASSERT_FALSE(this->fullContainer.try_push_front(6));
}

TYPED_TEST_P(PushFrontTests, PushIncrementsSize)
{
    for (auto pushMethod : this->PushFuncs) { this->TestPushIncrement(this->emptyContainer, pushMethod); }
}

REGISTER_TYPED_TEST_SUITE_P(PushFrontTests,
    WhenNotFullTryPushReturnsTrue,
    WhenFullClearEmptiesContainer,
    WhenFullPushFrontThrows,
    WhenFullTryPushFrontReturnsFalse,
    PushIncrementsSize
);

}