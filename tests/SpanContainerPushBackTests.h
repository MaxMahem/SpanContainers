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

	/// @brief Test fixture for testing Span containers with push back ability
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerPushBackTestFixture 
	: public SpanContainerPushTestFixture<TContainer, std::back_insert_iterator<TContainer>> { };

TYPED_TEST_SUITE_P(SpanContainerPushBackTestFixture);

TYPED_TEST_P(SpanContainerPushBackTestFixture, PushBackIncrementsSize) 
{
	auto initialSize = this->emptyContainer.size();

	this->emptyContainer.push_back(1);

	auto incrementedSize = this->emptyContainer.size();

	ASSERT_EQ(initialSize + 1, incrementedSize);
}

TYPED_TEST_P(SpanContainerPushBackTestFixture, WhenNotFullTryPushReturnsTrue)
{
	ASSERT_FALSE(this->emptyContainer.full());
	ASSERT_TRUE(this->emptyContainer.try_push_back(1));
}

TYPED_TEST_P(SpanContainerPushBackTestFixture, WhenFullSizeEqCapacity)
{
	ASSERT_EQ(this->fullContainer.size(), this->fullContainer.capacity());
}

TYPED_TEST_P(SpanContainerPushBackTestFixture, WhenFullClearEmptiesContainer)
{
	this->fullContainer.clear();

	ASSERT_THAT(this->fullContainer, ::testing::IsEmpty());
}

TYPED_TEST_P(SpanContainerPushBackTestFixture, WhenFullPushBackThrows)
{
	ASSERT_THROW(this->fullContainer.push_back(6), FullContainerError);
}

TYPED_TEST_P(SpanContainerPushBackTestFixture, WhenFullTryPushBackReturnsFalse)
{
	ASSERT_FALSE(this->fullContainer.try_push_back(6));
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerPushBackTestFixture,
	PushBackIncrementsSize,
	WhenNotFullTryPushReturnsTrue,
	WhenFullSizeEqCapacity,
	WhenFullClearEmptiesContainer,
	WhenFullPushBackThrows,
	WhenFullTryPushBackReturnsFalse
);

}