#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanDequeue.h"
#include "SpanStack.h"
#include "SpanQueue.h"

#include "TypedContainerTests.h"
#include "ContainerTestAdaptor.h"

namespace SpanContainers::Tests {

using Stack = SpanStack<int, 5>;
using StackTestAdaptor = ContainerTestAdaptor<Stack, PushBack<Stack>, PopBack<Stack>, LIFO>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, TypedContainerTests, StackTestAdaptor);

using Queue = SpanQueue<int, 5>;
using QueueTestAdaptor = ContainerTestAdaptor<Queue, PushBack<Queue>, PopFront<Queue>, FIFO>;
INSTANTIATE_TYPED_TEST_SUITE_P(QueueTests, TypedContainerTests, QueueTestAdaptor);

using Dequeue = SpanDequeue<int, 5>;
using DequePushBackPopFrontTestAdaptor = ContainerTestAdaptor<Dequeue, PushBack<Dequeue>, PopFront<Dequeue>, FIFO, Ascending>;
using DequePushBackPopBackTestAdaptor = ContainerTestAdaptor<Dequeue, PushBack<Dequeue>, PopBack<Dequeue>, LIFO, Ascending>;
using DequePushFrontPopFrontTestAdaptor = ContainerTestAdaptor<Dequeue, PushFront<Dequeue>, PopFront<Dequeue>, LIFO, Descending>;
using DequePushFrontPopBackTestAdaptor = ContainerTestAdaptor<Dequeue, PushFront<Dequeue>, PopBack<Dequeue>, FIFO, Descending>;

using DequeTestTypes = testing::Types<
    DequePushBackPopFrontTestAdaptor,
    DequePushBackPopBackTestAdaptor,
    DequePushFrontPopFrontTestAdaptor,
    DequePushFrontPopBackTestAdaptor
>;

INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, TypedContainerTests, DequeTestTypes);

}