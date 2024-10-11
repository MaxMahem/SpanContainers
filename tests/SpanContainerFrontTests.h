#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ContainerErrors.h"
#include "SpanContainerBaseTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push front ability
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerFrontTests : public SpanContainerBaseTests<TContainer> { };

TYPED_TEST_SUITE_P(SpanContainerFrontTests);

TYPED_TEST_P(SpanContainerFrontTests, WhenEmptyFrontThrows)
{
	EXPECT_THROW((void) this->emptyContainer.front(), EmptyContainerError);
}

TYPED_TEST_P(SpanContainerFrontTests, WhenEmptyTryFrontNullptr)
{
	EXPECT_EQ(this->emptyContainer.try_front(), nullptr);
}

TYPED_TEST_P(SpanContainerFrontTests, WhenEmptyPopFrontThrows)
{
	EXPECT_THROW(this->emptyContainer.pop_front(), EmptyContainerError);
}

TYPED_TEST_P(SpanContainerFrontTests, WhenEmptyTryPopFrontFalse)
{
	EXPECT_FALSE(this->emptyContainer.try_pop_front());
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerFrontTests,
	WhenEmptyFrontThrows,
	WhenEmptyTryFrontNullptr,
	WhenEmptyPopFrontThrows,
	WhenEmptyTryPopFrontFalse
);

}