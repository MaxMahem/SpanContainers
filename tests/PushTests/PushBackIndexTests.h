#pragma once

#include <gtest/gtest.h>

#include "../ContainerFuncs/IndexFuncs.h"
#include "PushBackTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

template <typename Container>
class PushBackIndexTests : public PushBackTests<Container>, public IndexedFuncs<Container> { };

TYPED_TEST_SUITE_P(PushBackIndexTests);

TYPED_TEST_P(PushBackIndexTests, IndexIsCorrect)
{
    for (auto indexMethod : this->IndexFuncs) { 
        EXPECT_EQ(indexMethod(this->fullContainer, 0), NUMBER_FILL[0]); 
        EXPECT_EQ(indexMethod(this->fullContainer, TEST_EXTENT - 1), NUMBER_FILL[TEST_EXTENT - 1]);
    }
}

REGISTER_TYPED_TEST_SUITE_P(PushBackIndexTests, IndexIsCorrect);

}