
#include <functional>
#include <gtest/gtest.h>

#define SC_USE_EXCEPTIONS false
#include "SpanDequeue.h"
#include "SpanHeap.h"
#include "SpanStack.h"
#include "SpanQueue.h"

#include "ContainerTestAdaptor.h"
#include "TypedContainerDeathTests.h"

namespace SpanContainers::Tests {

using Stack = SpanStack<int, 5>;
using StackTestAdaptor = ContainerTestAdaptor<Stack, PushBack<Stack>, PopBack<Stack>, LIFO>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackDeathTest, TypedContainerDeathTest, StackTestAdaptor);

using Queue = SpanQueue<int, 5>;
using QueueTestAdaptor = ContainerTestAdaptor<Queue, PushBack<Queue>, PopFront<Queue>, FIFO>;
INSTANTIATE_TYPED_TEST_SUITE_P(QueueDeathTest, TypedContainerDeathTest, QueueTestAdaptor);

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

INSTANTIATE_TYPED_TEST_SUITE_P(DequeueDeathTest, TypedContainerDeathTest, DequeTestTypes);

using Heap = SpanHeap<int, 5>;
using HeapTestAdaptor = ContainerTestAdaptor<Heap, PushStraight<Heap>, PopBack<Heap>, std::greater<int>>;

INSTANTIATE_TYPED_TEST_SUITE_P(HeapDeathTest, TypedContainerDeathTest, HeapTestAdaptor);

}