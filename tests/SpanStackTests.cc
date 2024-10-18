#include <gtest/gtest.h>

#include "SpanStack.h"

#include "BaseTests.h"
#include "PushTests/PushBackIndexTests.h"
#include "PushTests/PushBackTests.h"
#include "GetPopTests/BackTests.h"
#include "GetPopTests/EmptyIndexTests.h"
#include "PushPopTests/PushBackPopBackTests.h"

namespace SpanContainers::Tests {

using TestSpanStack = ::testing::Types<SpanStack<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, BaseTests,            TestSpanStack);
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, PushBackIndexTests,   TestSpanStack);
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, PushBackTests,        TestSpanStack);
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, BackTests,            TestSpanStack);
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, EmptyIndexTests,      TestSpanStack);
INSTANTIATE_TYPED_TEST_SUITE_P(StackTests, PushBackPopBackTests, TestSpanStack);

}