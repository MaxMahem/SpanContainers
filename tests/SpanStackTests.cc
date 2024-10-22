#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanStack.h"

#include "TypedContainerTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using Stack = SpanStack<int, 5>;
using StackTestType = std::tuple<Stack, PushBackFuncs<Stack>, PopBackFuncs<Stack>, IndexFuncs<Stack>, std::greater<int>>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, TypedContainerTests, StackTestType);

}