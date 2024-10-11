#pragma once
#include <iterator>
#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ContainerErrors.h"
#include "SpanContainerBaseTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push front, pop front ability (dequeue)
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerPushFrontPopFrontTests : public SpanContainerBaseTests<TContainer> { };

TYPED_TEST_SUITE_P(SpanContainerPushFrontPopFrontTests);

TYPED_TEST_P(SpanContainerPushFrontPopFrontTests, PopDecrementsSize)
{
	this->emptyContainer.push_front(1);
	auto initialSize = this->emptyContainer.size();

	this->emptyContainer.pop_front();

	EXPECT_EQ(initialSize - 1, this->emptyContainer.size());
}

TYPED_TEST_P(SpanContainerPushFrontPopFrontTests, PushFrontPopFrontIsFILO)
{
	std::ranges::copy(numberFill(), std::front_inserter(this->emptyContainer));

	std::vector<int> values;
	while (int* front = this->emptyContainer.try_front()) {
		values.push_back(*front);
		this->emptyContainer.pop_front();
	}
	EXPECT_TRUE(std::ranges::equal(numberFill() | std::views::reverse, values));
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerPushFrontPopFrontTests,
	PopDecrementsSize,
	PushFrontPopFrontIsFILO
);

}