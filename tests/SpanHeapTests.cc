#include <array>
#include <ranges>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SpanHeap.h"

#include "BaseTests.h"
#include "GetPopTests/BackTests.h"
#include "PushTests/PushStraightTests.h"
#include "PushPopTests/PushPopTestFixture.h"
#include "PushPopFuncs/PopBackFuncs.h"
#include "PushPopFuncs/PushStraightFuncs.h"
#include "PushPopTests/PushStraightPopBackTests.h"

namespace SpanContainers::Tests {

using TestSpanHeap = ::testing::Types<SpanHeap<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, BaseTests,         TestSpanHeap);
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, PushStraightTests, TestSpanHeap);
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, BackTests,         TestSpanHeap);
// default heap is a max heap, which should compare the same as the LIFO tests.
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, PushStraightPopBackTests, TestSpanHeap);

class SpanHeapTests : public PushPopTestFixture<SpanHeap<int, TEST_EXTENT>>,
                      public PopBackFuncs<SpanHeap<int, TEST_EXTENT>>,
                      public PushStraightFuncs<SpanHeap<int, TEST_EXTENT>> { };

TEST_F(SpanHeapTests, MaxHeapTest) 
{
    SpanHeap<int, TEST_EXTENT>::BufferType<> buffer{};
    SpanHeap<int, TEST_EXTENT, std::greater<int>> minHeap(buffer);

    for (int value : NUMBER_FILL) { minHeap.push(value); }

    std::vector<int> values;
    while (!minHeap.empty()) {
        values.push_back(minHeap.back());
        minHeap.pop_back();
    }
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL));
}

struct CustomComparer { constexpr bool operator()(int a, int b) { return a < b; } };

TEST_F(SpanHeapTests, StaticCustomComparer)
{
    SpanHeap<int, TEST_EXTENT>::BufferType<> buffer{};
    SpanHeap<int, TEST_EXTENT, CustomComparer> minHeap{ buffer };

    for (int value : NUMBER_FILL) { minHeap.push(value); }

    std::vector<int> values;
    while (!minHeap.empty()) {
        values.push_back(minHeap.back());
        minHeap.pop_back();
    }
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
}

}