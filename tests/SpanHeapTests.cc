#include  <cstddef>
#include <functional>
#include <ranges>
#include <variant>

#include <gtest/gtest.h>

#include "SpanHeap.h"

#include "TypedContainerTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using MinHeap = SpanHeap<int, 5>;
using MinHeapTestAdaptor = ContainerTestAdaptor<MinHeap, PushStraight<MinHeap>, PopBack<MinHeap>, std::greater<int>>;

using MaxHeap = SpanHeap<int, 5, std::greater<int>>;
using MaxHeapTestAdaptor = ContainerTestAdaptor<MaxHeap, PushStraight<MaxHeap>, PopBack<MaxHeap>, std::less<int>>;

struct CustomComparer { constexpr bool operator()(int a, int b) { return a < b; } };

using CustomComparerHeap = SpanHeap<int, 5, CustomComparer>;
using CustomComparerHeapTestAdaptor = ContainerTestAdaptor<CustomComparerHeap, PushStraight<CustomComparerHeap>, PopBack<CustomComparerHeap>, std::greater<int>>;

using HeapTypes = ::testing::Types<MinHeapTestAdaptor, MaxHeapTestAdaptor, CustomComparerHeapTestAdaptor>;
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, TypedContainerTests, HeapTypes);

constexpr std::size_t LARGE_TEST_EXTENT = 30;
constexpr std::size_t LARGE_TEST_SPLIT = LARGE_TEST_EXTENT / 5;

using LargeHeap = SpanHeap<int, LARGE_TEST_EXTENT>;
using LargeHeapTestAdaptor = ContainerTestAdaptor<LargeHeap, PushStraight<LargeHeap>, PopBack<LargeHeap>, std::greater<int>>;

class LargeHeapTest : public TypedContainerTests<LargeHeapTestAdaptor> { };

TEST_F(LargeHeapTest, PushLargeAndSmallRange)
{
    // test the large value path (calls std::make_heap)
    ASSERT_GT(LARGE_TEST_SPLIT, this->emptyContainer.make_threshold());
    this->emptyContainer.try_push_range(LargeHeapTest::FILL | std::views::drop(LARGE_TEST_SPLIT));
    // test the small value path (calls std::push_heap)
    ASSERT_LE(LARGE_TEST_SPLIT, this->emptyContainer.make_threshold());
    this->emptyContainer.try_push_range(LargeHeapTest::FILL | std::views::take(LARGE_TEST_SPLIT));

    std::vector<int> values = this->Empty(this->emptyContainer, LargeHeapTest::PopFuncs::get, LargeHeapTest::PopFuncs::pop);
    EXPECT_THAT(values, ::testing::ElementsAreArray(LargeHeapTest::EXPECTED_RESULTS));
}

}