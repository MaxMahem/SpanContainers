#include <gtest/gtest.h>

#include "SpanDequeue.h"

#include "BaseTests.h"
#include "GetPopTests/EmptyIndexTests.h"
#include "GetPopTests/BackTests.h"
#include "GetPopTests/FrontTests.h"
#include "PushTests/PushBackTests.h"
#include "PushTests/PushBackIndexTests.h"
#include "PushTests/PushFrontTests.h"
#include "PushTests/PushFrontIndexTests.h"
#include "PushPopTests/PushBackPopBackTests.h"
#include "PushPopTests/PushBackPopFrontTests.h"
#include "PushPopTests/PushBackPopFrontIndexTests.h"
#include "PushPopTests/PushFrontPopBackTests.h"
#include "PushPopTests/PushFrontPopBackIndexTests.h"
#include "PushPopTests/PushFrontPopFrontTests.h"

namespace SpanContainers::Tests {

using TestSpanDequeue = ::testing::Types<SpanDequeue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, BaseTests,                  TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, EmptyIndexTests,            TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, FrontTests,                 TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackTests,              TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackIndexTests,         TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontTests,             TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontIndexTests,        TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackPopBackTests,       TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackPopFrontTests,      TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushBackPopFrontIndexTests, TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontPopBackTests,      TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontPopBackIndexTests, TestSpanDequeue);
INSTANTIATE_TYPED_TEST_SUITE_P(SpanDequeue, PushFrontPopFrontTests,     TestSpanDequeue);

}