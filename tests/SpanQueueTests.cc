#include <gtest/gtest.h>

#include "SpanQueue.h"

#include "BaseTests.h"
#include "PushTests/PushBackTests.h"
#include "GetPopTests/FrontTests.h"
#include "PushPopTests/PushBackPopFrontTests.h"

namespace SpanContainers::Tests {

using TestSpanQueue = ::testing::Types<SpanQueue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, BaseTests,             TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, FrontTests,            TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, PushBackTests,         TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, PushBackPopFrontTests, TestSpanQueue);

}