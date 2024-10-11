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

/// @brief Test fixture for testing Span containers with push back, pop front ability (queue, dequeue)
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerPushBackPopFrontTests : public SpanContainerBaseTests<TContainer> { };

TYPED_TEST_SUITE_P(SpanContainerPushBackPopFrontTests);

TYPED_TEST_P(SpanContainerPushBackPopFrontTests, PopDecrementsSize)
{
	this->emptyContainer.push_back(1);
	auto initialSize = this->emptyContainer.size();

	this->emptyContainer.pop_front();

	EXPECT_EQ(initialSize - 1, this->emptyContainer.size());
}

TYPED_TEST_P(SpanContainerPushBackPopFrontTests, PushBackPopFrontIsFIFO)
{
	std::ranges::copy(numberFill(), std::back_inserter(this->emptyContainer));

	std::vector<int> values;
	while (int* front = this->emptyContainer.try_front()) {
		values.push_back(*front);
		this->emptyContainer.pop_front();
	}
	EXPECT_TRUE(std::ranges::equal(numberFill(), values));
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerPushBackPopFrontTests,
	PopDecrementsSize,
	PushBackPopFrontIsFIFO
);

}