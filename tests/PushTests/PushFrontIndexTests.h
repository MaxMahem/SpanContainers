#pragma once

#include <gtest/gtest.h>

#include "../ContainerFuncs/IndexFuncs.h"
#include "PushFrontTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

template <typename Container>
class PushFrontIndexTests : public PushFrontTests<Container>, public IndexedFuncs<Container> { };

TYPED_TEST_SUITE_P(PushFrontIndexTests);

TYPED_TEST_P(PushFrontIndexTests, IndexIsCorrect)
{
    for (auto indexMethod : this->IndexFuncs) { 
        EXPECT_EQ(indexMethod(this->fullContainer, 0), NUMBER_FILL_REVERSE[0]);
        EXPECT_EQ(indexMethod(this->fullContainer, TEST_EXTENT - 1), NUMBER_FILL_REVERSE[TEST_EXTENT - 1]);
    }
}

REGISTER_TYPED_TEST_SUITE_P(PushFrontIndexTests, IndexIsCorrect);

}