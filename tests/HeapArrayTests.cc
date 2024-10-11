#include <span>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "internal\HeapArray.h"

namespace SpanContainers::Tests {

using namespace SpanContainers::internal;

constexpr std::size_t TEST_EXTENT = 10;

TEST(HeapArrayTests, HeapArrayToSpanConversion) 
{
    HeapArray<int, TEST_EXTENT> heapArray;

    std::span<int, TEST_EXTENT> spanFromHeapArray = heapArray;

    EXPECT_THAT(spanFromHeapArray, ::testing::SizeIs(TEST_EXTENT));
}

}