#include <gtest/gtest.h>

#define SPAN_CONTAINERS_USE_EXCEPTIONS false
#include "SpanHeap.h"

#include "PushTests/PushStraightDeathTests.h"
#include "GetPopTests/BackDeathTests.h"

namespace SpanContainers::Tests {

using TestSpanHeap = ::testing::Types<SpanHeap<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, PushStraightDeathTest, TestSpanHeap);
INSTANTIATE_TYPED_TEST_SUITE_P(HeapTests, BackDeathTest,         TestSpanHeap);

}