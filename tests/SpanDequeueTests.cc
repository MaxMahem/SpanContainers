#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanDequeue.h"

#include "TypedContainerTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

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

INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, TypedContainerTests, DequeTestTypes);

}