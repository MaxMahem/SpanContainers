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
#include "ContainerFuncs/PopBackFuncs.h"
#include "ContainerFuncs/PushStraightFuncs.h"
#include "PushPopTests/PushStraightPopBackTests.h"

namespace SpanContainers::Tests {

using TestSpanHeap = ::testing::Types<SpanHeap<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, BaseTests,         TestSpanHeap);
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, PushStraightTests, TestSpanHeap);
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, BackTests,         TestSpanHeap);
// default heap is a max heap, which should compare the same as the LIFO tests.
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, PushStraightPopBackTests, TestSpanHeap);

class SpanMaxHeapTests : public PushStraightPopBackTests<SpanHeap<int, TEST_EXTENT>> { };

TEST_F(SpanMaxHeapTests, PushPopIsMaxHeap)
{
    for (auto& pushMethod : this->PushFuncs) {
        std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, pushMethod, this->get, this->pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
    }
}

TEST_F(SpanMaxHeapTests, PushPopAfterClearIsMaxHeap)
{
    // partially fill then clear
    for (int value : NUMBER_FILL | std::views::take(3)) { this->emptyContainer.push(value); }
    this->emptyContainer.clear();

    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
}

class SpanMinHeapTests : public PushStraightPopBackTests<SpanHeap<int, TEST_EXTENT, std::greater<int>>> { };

TEST_F(SpanMinHeapTests, PushPopMinHeapTest)
{
    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL));
}

struct CustomComparer { constexpr bool operator()(int a, int b) { return a < b; } };

class SpanCustomComparerHeapTests : public PushStraightPopBackTests<SpanHeap<int, TEST_EXTENT, CustomComparer>> { };

TEST_F(SpanCustomComparerHeapTests, PushPopStaticCustomComparer)
{
    std::vector<int> values = this->BuildPushPopVector(this->emptyContainer, NUMBER_FILL, this->push, this->get, this->pop);
    EXPECT_THAT(values, ::testing::ElementsAreArray(NUMBER_FILL_REVERSE));
}

}