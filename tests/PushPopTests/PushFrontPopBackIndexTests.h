#pragma once

#include <gtest/gtest.h>

#include "../BaseTests.h"
#include "../ContainerFuncs/IndexFuncs.h"
#include "PushFrontPopBackTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

template <typename Container>
class PushFrontPopBackIndexTests : public PushFrontPopBackTests<Container>,
                                   public IndexedFuncs<Container> { };

TYPED_TEST_SUITE_P(PushFrontPopBackIndexTests);

TYPED_TEST_P(PushFrontPopBackIndexTests, PushPopWrappingIndexCorrect)
{
    // partially fill then pop-empty to force wrap
    for (int value : NUMBER_FILL | std::views::take(TEST_EXTENT - 1)) { this->push(this->emptyContainer, value); }
    for (int iteration = TEST_EXTENT - 1; iteration > 0; --iteration) { this->pop(this->emptyContainer); }
    for (int value : NUMBER_FILL) { this->push(this->emptyContainer, value); }

    for (auto indexMethod : this->IndexFuncs) {
        EXPECT_EQ(indexMethod(this->emptyContainer, 0), NUMBER_FILL_REVERSE[0]);
        EXPECT_EQ(indexMethod(this->emptyContainer, TEST_EXTENT - 1), NUMBER_FILL_REVERSE[TEST_EXTENT - 1]);
    }
}

REGISTER_TYPED_TEST_SUITE_P(PushFrontPopBackIndexTests, PushPopWrappingIndexCorrect);

}