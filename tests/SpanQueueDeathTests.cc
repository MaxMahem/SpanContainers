#include <gtest/gtest.h>

#define SPAN_CONTAINERS_USE_EXCEPTIONS false
#include "SpanQueue.h"

#include "PushTests/PushBackDeathTests.h"
#include "GetPopTests/FrontDeathTests.h"
#include "GetPopTests/EmptyIndexDeathTests.h"

namespace SpanContainers::Tests {

using TestSpanQueue = ::testing::Types<SpanQueue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(QueueTests, PushBackDeathTest,   TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(QueueTests, FrontDeathTest,      TestSpanQueue);
INSTANTIATE_TYPED_TEST_SUITE_P(QueueTests, EmptyIndexDeathTest, TestSpanQueue);

}