#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ContainerErrors.h"
#include "SpanContainerBaseTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push back ability
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerBackTests : public SpanContainerBaseTests<TContainer> { };

TYPED_TEST_SUITE_P(SpanContainerBackTests);

TYPED_TEST_P(SpanContainerBackTests, WhenEmptyBackThrows)
{
	EXPECT_THROW((void) this->emptyContainer.back(), EmptyContainerError);
}

TYPED_TEST_P(SpanContainerBackTests, WhenEmptyTryBackNullptr)
{
	EXPECT_EQ(this->emptyContainer.try_back(), nullptr);
}

TYPED_TEST_P(SpanContainerBackTests, WhenEmptyPopBackThrows)
{
	EXPECT_THROW(this->emptyContainer.pop_back(), EmptyContainerError);
}

TYPED_TEST_P(SpanContainerBackTests, WhenEmptyTryPopBackFalse)
{
	EXPECT_FALSE(this->emptyContainer.try_pop_back());
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerBackTests,
	WhenEmptyBackThrows,
	WhenEmptyTryBackNullptr,
	WhenEmptyPopBackThrows,
	WhenEmptyTryPopBackFalse
);

}