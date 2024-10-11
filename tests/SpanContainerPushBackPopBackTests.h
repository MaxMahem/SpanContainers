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

/// @brief Test fixture for testing Span containers with push back, pop back ability (stack, dequeue)
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerPushBackPopBackTests : public SpanContainerBaseTests<TContainer> { };

TYPED_TEST_SUITE_P(SpanContainerPushBackPopBackTests);

TYPED_TEST_P(SpanContainerPushBackPopBackTests, PopDecrementsSize)
{
	this->emptyContainer.push_back(1);
	auto initialSize = this->emptyContainer.size();

	this->emptyContainer.pop_back();

	EXPECT_EQ(initialSize - 1, this->emptyContainer.size());
}

TYPED_TEST_P(SpanContainerPushBackPopBackTests, PushBackPopBackIsFILO)
{
	std::ranges::copy(numberFill(), std::back_inserter(this->emptyContainer));

	std::vector<int> values;
	while (int* back = this->emptyContainer.try_back()) {
		values.push_back(*back);
		this->emptyContainer.pop_back();
	}
	EXPECT_TRUE(std::ranges::equal(numberFill() | std::views::reverse, values));
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerPushBackPopBackTests,
	PopDecrementsSize,
	PushBackPopBackIsFILO
);

}