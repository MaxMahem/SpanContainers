#include <gtest/gtest.h>

#include "SpanDequeue.h"

#include "BaseTests.h"
#include "GetPopTests/BackTests.h"
#include "GetPopTests/FrontTests.h"
#include "PushTests/PushBackTests.h"
#include "PushTests/PushFrontTests.h"
#include "PushPopTests/PushBackPopBackTests.h"
#include "PushPopTests/PushBackPopFrontTests.h"
#include "PushPopTests/PushFrontPopBackTests.h"
#include "PushPopTests/PushFrontPopFrontTests.h"

namespace SpanContainers::Tests {

using TestSpanDequeue = ::testing::Types<SpanDequeue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, BaseTests,              TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, FrontTests,             TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackTests,          TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontTests,         TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackPopBackTests,   TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackPopFrontTests,  TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontPopBackTests,  TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontPopFrontTests, TestSpanDequeue);

}