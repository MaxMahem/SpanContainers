#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Errors/FullContainerError.h"

#include "../PushPopFuncs/PushBackFuncs.h"
#include "PushTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push back ability
/// @tparam Container the type of the container.
template <typename Container>
class PushBackTests : public PushTestFixture<Container, std::back_insert_iterator<Container>>, 
                      public PushBackFuncs<Container>{ };

TYPED_TEST_SUITE_P(PushBackTests);

TYPED_TEST_P(PushBackTests, WhenNotFullTryPushReturnsTrue)
{
    ASSERT_FALSE(this->emptyContainer.full());
    ASSERT_TRUE(this->emptyContainer.try_push_back(1));
    ASSERT_TRUE(this->emptyContainer.try_emplace_back(1));
}

TYPED_TEST_P(PushBackTests, WhenFullClearEmptiesContainer)
{
    this->fullContainer.clear();
    ASSERT_THAT(this->fullContainer, ::testing::IsEmpty());
}

TYPED_TEST_P(PushBackTests, WhenFullPushBackThrows)
{
    ASSERT_THROW(this->fullContainer.push_back(6),     FullContainerError);
    ASSERT_THROW(this->fullContainer.emplace_back(6),  FullContainerError);
}

TYPED_TEST_P(PushBackTests, WhenFullTryPushBackReturnsFalse)
{
    ASSERT_FALSE(this->fullContainer.try_push_back(6));
    ASSERT_FALSE(this->fullContainer.try_emplace_back(6));
}

TYPED_TEST_P(PushBackTests, PushIncrementsSize)
{
    for (auto pushMethod : this->PushFuncs) { this->TestPushIncrement(this->emptyContainer, pushMethod); }
}

REGISTER_TYPED_TEST_SUITE_P(PushBackTests,
    WhenNotFullTryPushReturnsTrue,
    WhenFullClearEmptiesContainer,
    WhenFullPushBackThrows,
    WhenFullTryPushBackReturnsFalse,
    PushIncrementsSize
);

}