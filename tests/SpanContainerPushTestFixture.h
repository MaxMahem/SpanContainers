#pragma once
#include <array>
#include <ranges>

#include <gtest/gtest.h>

#include "SpanContainerBaseTests.h"

namespace SpanContainers::Tests {

/// @brief Test fixture for testing Span container with push based insertion
/// @tparam TContainer the type of the container.
/// @tparam Inserter the type of inserter to use when creating a full container
template <typename TContainer, typename Inserter>
class SpanContainerPushTestFixture : public SpanContainerBaseTests<TContainer> {
	std::array<int, 5> array{};

protected:
	TContainer fullContainer;

	SpanContainerPushTestFixture() : fullContainer(array)
	{
		std::ranges::copy(numberFill(), Inserter{fullContainer});
	}

	void SetUp() override 
	{
		SpanContainerBaseTests<TContainer>::SetUp();
		ASSERT_TRUE(fullContainer.full());
	}
};

}