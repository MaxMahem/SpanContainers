#include <functional>
#include <variant>

#include <gtest/gtest.h>

#define SC_USE_EXCEPTIONS false
#include "SpanDequeue.h"

#include "TypedContainerDeathTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using Dequeue = SpanDequeue<int, 5>;
using DequePushBackPopFront  = std::tuple<Dequeue, PushBackFuncs<Dequeue>,  PopFrontFuncs<Dequeue>, IndexFuncs<Dequeue>, std::less<int>>;
using DequePushFrontPopBack  = std::tuple<Dequeue, PushFrontFuncs<Dequeue>, PopBackFuncs<Dequeue>,  NoIndex,             std::less<int>>;

using DequeTestTypes = testing::Types<DequePushBackPopFront, DequePushFrontPopBack>;

INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, TypedContainerDeathTest, DequeTestTypes);

}