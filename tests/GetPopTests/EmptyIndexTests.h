#pragma once

#include <gtest/gtest.h>

#include "Errors/EmptyContainerError.h"

#include "../BaseTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with index trait
/// @tparam Container the type of the container.
template <typename Container> class EmptyIndexTests : public BaseTests<Container> { };

TYPED_TEST_SUITE_P(EmptyIndexTests);

TYPED_TEST_P(EmptyIndexTests, WhenEmptySubscriptThrows) { ASSERT_THROW((void) this->emptyContainer[0], std::out_of_range); }

TYPED_TEST_P(EmptyIndexTests, WhenEmptyAtNullptr) { ASSERT_EQ(this->emptyContainer.at(0), nullptr); }

REGISTER_TYPED_TEST_SUITE_P(EmptyIndexTests, WhenEmptySubscriptThrows, WhenEmptyAtNullptr);

}