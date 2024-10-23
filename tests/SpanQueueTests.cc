#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanQueue.h"

#include "TypedContainerTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using Queue = SpanQueue<int, 5>;
using QueueTestAdaptor = ContainerTestAdaptor<Queue, PushBack<Queue>, PopFront<Queue>, FIFO>;
INSTANTIATE_TYPED_TEST_SUITE_P(QueueTests, TypedContainerTests, QueueTestAdaptor);

}