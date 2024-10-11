#pragma once
#include <array>
#include <format>
#include <ranges>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SpanContainerFormat.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

constexpr size_t TEST_EXTENT = 5;

/// @brief Test fixture for testing base behavior of Span container types.
/// @tparam TContainer the type of the container.
template <typename TContainer>
class SpanContainerBaseTests : public testing::Test
{
	std::array<int, TEST_EXTENT> array{};

protected:
	TContainer emptyContainer;

	SpanContainerBaseTests() : emptyContainer(array) { }

	void SetUp() override
	{
		testing::Test::SetUp();
		ASSERT_THAT(emptyContainer, ::testing::IsEmpty());
	}
};

consteval auto numberFill() { return std::views::iota(1, int{ TEST_EXTENT } + 1); }

TYPED_TEST_SUITE_P(SpanContainerBaseTests);

TYPED_TEST_P(SpanContainerBaseTests, IsTrivallyCopyable)
{
	ASSERT_TRUE(std::is_trivially_copyable<TypeParam>::value);
}

TYPED_TEST_P(SpanContainerBaseTests, CopyConstructor)
{
	TypeParam copy{ this->emptyContainer };
	ASSERT_EQ(this->emptyContainer.data().data(), copy.data().data());
}

TYPED_TEST_P(SpanContainerBaseTests, CopyAssignment)
{
	TypeParam copy = this->emptyContainer;
	ASSERT_EQ(this->emptyContainer.data().data(), copy.data().data());
}

TYPED_TEST_P(SpanContainerBaseTests, StdFormatProducesIntendedResults)
{
	std::string formattedValue = std::format("{}", this->emptyContainer);

	std::string matchingRegex = std::format(
		"{}\\[{}/{}\\]",
		this->emptyContainer.TYPE_NAME,
		this->emptyContainer.size(),
		this->emptyContainer.capacity()
	);	

	ASSERT_THAT(formattedValue, ::testing::MatchesRegex(matchingRegex));
}

REGISTER_TYPED_TEST_SUITE_P(SpanContainerBaseTests,
	IsTrivallyCopyable, 
	CopyConstructor,
	CopyAssignment,
	StdFormatProducesIntendedResults
);

}