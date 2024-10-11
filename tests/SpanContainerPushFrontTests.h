#pragma once
#include <iterator>
#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ContainerErrors.h"
#include "SpanContainerPushTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push front ability
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerPushFrontTestFixture 
	: public SpanContainerPushTestFixture<TContainer, std::front_insert_iterator<TContainer>> { };

TYPED_TEST_SUITE_P(SpanContainerPushFrontTestFixture);

TYPED_TEST_P(SpanContainerPushFrontTestFixture, PushFrontIncrementsSize) 
{
	auto initialSize = this->emptyContainer.size();

	this->emptyContainer.push_front(1);

	auto incrementedSize = this->emptyContainer.size();

	ASSERT_EQ(initialSize + 1, incrementedSize);
}

TYPED_TEST_P(SpanContainerPushFrontTestFixture, WhenNotFullTryPushReturnsTrue)
{
	ASSERT_FALSE(this->emptyContainer.full());
	ASSERT_TRUE(this->emptyContainer.try_push_front(1));
}

TYPED_TEST_P(SpanContainerPushFrontTestFixture, WhenFullSizeEqCapacity)
{
	ASSERT_EQ(this->fullContainer.size(), this->fullContainer.capacity());
}

TYPED_TEST_P(SpanContainerPushFrontTestFixture, WhenFullClearEmptiesContainer)
{
	this->fullContainer.clear();

	ASSERT_THAT(this->fullContainer, ::testing::IsEmpty());
}

TYPED_TEST_P(SpanContainerPushFrontTestFixture, WhenFullPushFrontThrows)
{
	ASSERT_THROW(this->fullContainer.push_front(6), FullContainerError);
}

TYPED_TEST_P(SpanContainerPushFrontTestFixture, WhenFullTryPushFrontReturnsFalse)
{
	ASSERT_FALSE(this->fullContainer.try_push_front(6));
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerPushFrontTestFixture,
	PushFrontIncrementsSize,
	WhenNotFullTryPushReturnsTrue,
	WhenFullSizeEqCapacity,
	WhenFullClearEmptiesContainer,
	WhenFullPushFrontThrows,
	WhenFullTryPushFrontReturnsFalse
);

}