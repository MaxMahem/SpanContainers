#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanDequeue.h"
#include "SpanStack.h"
#include "SpanQueue.h"

#include "CircularContainerTests.h"
#include "ContainerTestAdaptor.h"
#include "IndexedContainerTests.h"
#include "TypedContainerTests.h"

namespace SpanContainers::Tests {

using Stack = SpanStack<int, 5>;
using StackTestAdaptor = ContainerTestAdaptor<Stack, PushBack<Stack>, PopBack<Stack>, LIFO, Ascending>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests,      TypedContainerTests,   StackTestAdaptor);
INSTANTIATE_TYPED_TEST_SUITE_P(StackIndexTests, IndexedContainerTests, StackTestAdaptor);

using Queue = SpanQueue<int, 5>;
using QueueTestAdaptor = ContainerTestAdaptor<Queue, PushBack<Queue>, PopFront<Queue>, FIFO, Ascending>;
INSTANTIATE_TYPED_TEST_SUITE_P(QueueTests,      TypedContainerTests,    QueueTestAdaptor);
INSTANTIATE_TYPED_TEST_SUITE_P(QueueIndexTests, IndexedContainerTests,  QueueTestAdaptor);
INSTANTIATE_TYPED_TEST_SUITE_P(QueueWrapTests,  CircularContainerTests, QueueTestAdaptor);

using Dequeue = SpanDequeue<int, 5>;
using DequePushBackPopFrontTestAdaptor  = ContainerTestAdaptor<Dequeue, PushBack<Dequeue>,  PopFront<Dequeue>, FIFO, Ascending>;
using DequePushBackPopBackTestAdaptor   = ContainerTestAdaptor<Dequeue, PushBack<Dequeue>,  PopBack<Dequeue>,  LIFO, Ascending>;
using DequePushFrontPopFrontTestAdaptor = ContainerTestAdaptor<Dequeue, PushFront<Dequeue>, PopFront<Dequeue>, LIFO, Descending>;
using DequePushFrontPopBackTestAdaptor  = ContainerTestAdaptor<Dequeue, PushFront<Dequeue>, PopBack<Dequeue>,  FIFO, Descending>;

using DequeTestTypes = testing::Types<
    DequePushBackPopFrontTestAdaptor,
    DequePushBackPopBackTestAdaptor,
    DequePushFrontPopFrontTestAdaptor,
    DequePushFrontPopBackTestAdaptor
>;

INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests,      TypedContainerTests,    DequeTestTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(DequeueIndexTests, IndexedContainerTests,  DequeTestTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(DequeueWrapTests,  CircularContainerTests, DequeTestTypes);

}