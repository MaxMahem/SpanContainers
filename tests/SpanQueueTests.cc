#include <gtest/gtest.h>

#include "SpanQueue.h"

#include "BaseTests.h"
#include "PushTests/PushBackTests.h"
#include "PushTests/PushBackIndexTests.h"
#include "GetPopTests/EmptyIndexTests.h"
#include "GetPopTests/FrontTests.h"
#include "PushPopTests/PushBackPopFrontTests.h"
#include "PushPopTests/PushBackPopFrontIndexTests.h"

namespace SpanContainers::Tests {

using TestSpanQueue = ::testing::Types<SpanQueue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, BaseTests,                  TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, FrontTests,                 TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, EmptyIndexTests,            TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, PushBackTests,              TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, PushBackIndexTests,         TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, PushBackPopFrontTests,      TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanQueue, PushBackPopFrontIndexTests, TestSpanQueue);

}