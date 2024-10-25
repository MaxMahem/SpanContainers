#include <functional>

#include <gtest/gtest.h>

#define SC_USE_EXCEPTIONS false
#include "SpanQueue.h"

#include "ContainerTestTraits.h"
#include "TypedContainerDeathTests.h"

namespace SpanContainers::Tests {

using Queue = SpanQueue<int, 5>;
using QueueTestType = std::tuple<Queue, PushBackFuncs<Queue>, PopFrontFuncs<Queue>, IndexFuncs<Queue>, std::greater<int>>;
INSTANTIATE_TYPED_TEST_SUITE_P(QueueDeathTests, TypedContainerDeathTest, QueueTestType);

}