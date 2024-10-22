#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanDequeue.h"

#include "TypedContainerTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using Dequeue = SpanDequeue<int, 5>;
using DequePushBackPopFront  = std::tuple<Dequeue, PushBackFuncs<Dequeue>,  PopFrontFuncs<Dequeue>, IndexFuncs<Dequeue>, std::less<int>>;
using DequePushBackPopBack   = std::tuple<Dequeue, PushBackFuncs<Dequeue>,  PopBackFuncs<Dequeue>,  NoIndex,             std::greater<int>>;
using DequePushFrontPopFront = std::tuple<Dequeue, PushFrontFuncs<Dequeue>, PopFrontFuncs<Dequeue>, IndexFuncs<Dequeue>, std::greater<int>>;
using DequePushFrontPopBack  = std::tuple<Dequeue, PushFrontFuncs<Dequeue>, PopBackFuncs<Dequeue>,  NoIndex,             std::less<int>>;

using DequeTestTypes = testing::Types<DequePushBackPopFront, DequePushBackPopBack, DequePushFrontPopFront, DequePushFrontPopBack>;

INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, TypedContainerTests, DequeTestTypes);

}