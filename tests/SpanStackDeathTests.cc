
#include <functional>
#include <gtest/gtest.h>

#define SPAN_CONTAINERS_USE_EXCEPTIONS false
#include "SpanStack.h"

#include "ContainerTestTraits.h"
#include "TypedContainerDeathTests.h"

namespace SpanContainers::Tests {

using Stack = SpanStack<int, 5>;
using StackTestType = std::tuple<Stack, PushBackFuncs<Stack>, PopBackFuncs<Stack>, IndexFuncs<Stack>, std::greater<int>>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackDeathTests, TypedContainerDeathTest, StackTestType);

}