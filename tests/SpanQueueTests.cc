#include <algorithm>
#include <array>
#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SpanQueue.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

constexpr size_t TEST_EXTENT = 5;
typedef SpanQueue<int, TEST_EXTENT> TestSpanQueue;

class SpanQueueTest : public testing::Test 
{
	std::array<int, TEST_EXTENT> array{};

protected:
	TestSpanQueue spanQueue;

	SpanQueueTest() : spanQueue(array) { }
	
	constexpr auto numberFill() const { return std::views::iota(0, static_cast<int>(TEST_EXTENT)); };
};

TEST_F(SpanQueueTest, WhenEmptyFrontThrows) 
{
	ASSERT_THAT(spanQueue, ::testing::IsEmpty());
	EXPECT_THROW(static_cast<void>(spanQueue.front()), SpanContainers::EmptyContainerError);
}

TEST_F(SpanQueueTest, WhenEmptyTryFrontNullptr) 
{
	ASSERT_THAT(spanQueue, ::testing::IsEmpty());
	EXPECT_EQ(spanQueue.try_front(), nullptr);
}

TEST_F(SpanQueueTest, WhenEmptyPopFrontThrows) 
{
	ASSERT_THAT(spanQueue, ::testing::IsEmpty());
	EXPECT_THROW(spanQueue.pop_front(), EmptyContainerError);
}

TEST_F(SpanQueueTest, WhenEmptyTryPopFrontFalse) 
{
	ASSERT_THAT(spanQueue, ::testing::IsEmpty());
	EXPECT_FALSE(spanQueue.try_pop_front());
}

TEST_F(SpanQueueTest, PopFrontDecrementsSize)
{
	spanQueue.push_back(1);
	TestSpanQueue::size_type initialHeapSize = spanQueue.size();

	spanQueue.pop_front();
	TestSpanQueue::size_type decrementedHeapSize = spanQueue.size();

	EXPECT_EQ(initialHeapSize - 1, decrementedHeapSize);
}

TEST_F(SpanQueueTest, PopFrontIsFIFO)
{
	std::ranges::copy(numberFill(), std::back_inserter(spanQueue));

	std::vector<int> queueValues;
	while (int* back = spanQueue.try_front()) {
		queueValues.push_back(*back);
		spanQueue.pop_front();
	}

	EXPECT_TRUE(std::ranges::equal(numberFill(), queueValues));
}

TEST_F(SpanQueueTest, WrapAroundBehavior)
{
	// move write index to just before end
	const TestSpanQueue::size_type fillSize = numberFill().size() - 1;
	std::ranges::copy(numberFill() | std::views::take(fillSize), std::back_inserter(spanQueue));

	// move read index to just before end
	for (TestSpanQueue::size_type i = 0; i < fillSize; ++i) { spanQueue.pop_front(); }
	std::ranges::copy(numberFill(), std::back_inserter(spanQueue));

	// another fill/empty forces wrap around
	std::vector<int> queueValues;
	while (int* back = spanQueue.try_front()) {
		queueValues.push_back(*back);
		spanQueue.pop_front();
	}

	EXPECT_TRUE(std::ranges::equal(numberFill(), queueValues));
}

}