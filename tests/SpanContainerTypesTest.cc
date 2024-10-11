#include <gtest/gtest.h>

#include "SpanContainerBaseTests.h"
#include "SpanContainerBackTests.h"
#include "SpanContainerFrontTests.h"
#include "SpanContainerPushBackTests.h"
#include "SpanContainerPushBackPopBackTests.h"
#include "SpanContainerPushBackPopFrontTests.h"
#include "SpanContainerPushFrontTests.h"
#include "SpanContainerPushFrontPopBackTests.h"
#include "SpanContainerPushFrontPopFrontTests.h"
#include "SpanDequeue.h"
#include "SpanQueue.h"
#include "SpanStack.h"

namespace SpanContainers::Tests {

using AllSpanContainerTypes = ::testing::Types<
	SpanDequeue<int, TEST_EXTENT>,
	SpanQueue<int, TEST_EXTENT>, 
	SpanStack<int, TEST_EXTENT>
>;
INSTANTIATE_TYPED_TEST_SUITE_P(BaseTests, SpanContainerBaseTests, AllSpanContainerTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(PushBackTests, SpanContainerPushBackTestFixture, AllSpanContainerTypes);

using PushFrontContainerTypes = ::testing::Types<SpanDequeue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(PushFrontTests, SpanContainerPushFrontTestFixture, PushFrontContainerTypes);

using PopFrontContainerTypes = ::testing::Types<
	SpanDequeue<int, TEST_EXTENT>,
	SpanQueue<int, TEST_EXTENT>
>;
INSTANTIATE_TYPED_TEST_SUITE_P(FrontTests, SpanContainerFrontTests, PopFrontContainerTypes);

using PopBackContainerTypes = ::testing::Types<
	SpanDequeue<int, TEST_EXTENT>,
	SpanStack<int, TEST_EXTENT>
>;
INSTANTIATE_TYPED_TEST_SUITE_P(BackTests, SpanContainerBackTests, PopBackContainerTypes);

using PushBackPopBackContainerTypes = ::testing::Types<
	SpanDequeue<int, TEST_EXTENT>,
	SpanStack<int, TEST_EXTENT>
>;
INSTANTIATE_TYPED_TEST_SUITE_P(PushBackPopBackTests, SpanContainerPushBackPopBackTests, PushBackPopBackContainerTypes);

using PushBackPopFrontContainerTypes = ::testing::Types<
	SpanDequeue<int, TEST_EXTENT>,
	SpanQueue<int, TEST_EXTENT>
>;
INSTANTIATE_TYPED_TEST_SUITE_P(PushBackPopFrontTests, SpanContainerPushBackPopFrontTests, PushBackPopFrontContainerTypes);

using PushFrontPopFrontContainerTypes = ::testing::Types<SpanDequeue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(PushFrontPopFrontTests, SpanContainerPushFrontPopFrontTests, PushFrontPopFrontContainerTypes);

using PushFrontPopBackContainerTypes = ::testing::Types<SpanDequeue<int, TEST_EXTENT>>;
INSTANTIATE_TYPED_TEST_SUITE_P(PushFrontPopBackTests, SpanContainerPushFrontPopBackTests, PushFrontPopBackContainerTypes);
}