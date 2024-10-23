#include <functional>
#include <variant>

#include <gtest/gtest.h>

#include "SpanStack.h"

#include "TypedContainerTests.h"
#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using Stack = SpanStack<int, 5>;
using StackTestAdaptor = ContainerTestAdaptor<Stack, PushBack<Stack>, PopBack<Stack>, LIFO>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, TypedContainerTests, StackTestAdaptor);

}