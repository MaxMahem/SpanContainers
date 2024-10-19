#include <gtest/gtest.h>

#define SPAN_CONTAINERS_USE_EXCEPTIONS false
#include "SpanDequeue.h"

#include "PushTests/PushBackDeathTests.h"
#include "PushTests/PushFrontDeathTests.h"
#include "GetPopTests/BackDeathTests.h"
#include "GetPopTests/FrontDeathTests.h"
#include "GetPopTests/EmptyIndexDeathTests.h"

namespace SpanContainers::Tests {

using TestSpanDequeue = ::testing::Types<SpanDequeue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, PushBackDeathTest,   TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, PushFrontDeathTest,  TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, BackDeathTest,       TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, FrontDeathTest,      TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(DequeueTests, EmptyIndexDeathTest, TestSpanDequeue);

}