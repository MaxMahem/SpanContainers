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

/// @brief Test fixture for testing Span containers with push front, pop back ability (dequeue)
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerPushFrontPopBackTests : public SpanContainerBaseTests<TContainer> { };

TYPED_TEST_SUITE_P(SpanContainerPushFrontPopBackTests);

TYPED_TEST_P(SpanContainerPushFrontPopBackTests, PopDecrementsSize)
{
	this->emptyContainer.push_front(1);
	auto initialSize = this->emptyContainer.size();

	this->emptyContainer.pop_back();

	EXPECT_EQ(initialSize - 1, this->emptyContainer.size());
}

TYPED_TEST_P(SpanContainerPushFrontPopBackTests, PushFrontPopBackIsFIFO)
{
	std::ranges::copy(numberFill(), std::front_inserter(this->emptyContainer));

	std::vector<int> values;
	while (int* back = this->emptyContainer.try_back()) {
		values.push_back(*back);
		this->emptyContainer.pop_back();
	}
	EXPECT_TRUE(std::ranges::equal(numberFill(), values));
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerPushFrontPopBackTests,
	PopDecrementsSize,
	PushFrontPopBackIsFIFO
);

}