#include <functional>

#include <gtest/gtest.h>

#define SPAN_CONTAINERS_USE_EXCEPTIONS false
#include "SpanHeap.h"

#include "ContainerTestTraits.h"
#include "TypedContainerDeathTests.h"

namespace SpanContainers::Tests {

using Heap = SpanHeap<int, 5>;
using HeapTestType = std::tuple<Heap, PushStraightFuncs<Heap>, PopBackFuncs<Heap>, NoIndex, std::greater<int>>;
INSTANTIATE_TYPED_TEST_SUITE_P(HeapDeathTests, TypedContainerDeathTest, HeapTestType);

}