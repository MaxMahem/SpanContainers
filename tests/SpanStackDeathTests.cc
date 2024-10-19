#include <gtest/gtest.h>

#define SPAN_CONTAINERS_USE_EXCEPTIONS false
#include "SpanStack.h"

#include "PushTests/PushBackDeathTests.h"
#include "GetPopTests/BackDeathTests.h"
#include "GetPopTests/EmptyIndexDeathTests.h"

namespace SpanContainers::Tests {

using TestSpanStack = ::testing::Types<SpanStack<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, PushBackDeathTest,   TestSpanStack);
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, BackDeathTest,       TestSpanStack);
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, EmptyIndexDeathTest, TestSpanStack);

}