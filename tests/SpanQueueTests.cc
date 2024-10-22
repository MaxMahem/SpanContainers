#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanQueue.h"

#include "TypedContainerTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using Queue = SpanQueue<int, 5>;
using QueueTestType = std::tuple<Queue, PushBackFuncs<Queue>, PopFrontFuncs<Queue>, IndexFuncs<Queue>, std::less<int>>;
INSTANTIATE_TYPED_TEST_SUITE_P(QueueTests, TypedContainerTests, QueueTestType);

}