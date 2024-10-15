#pragma once

#include <gtest/gtest.h>

#include "Errors/EmptyContainerError.h"
#include "../BaseTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push front ability
/// @tparam Container the type of the container.
template <typename Container> class FrontTests : public BaseTests<Container> { };

TYPED_TEST_SUITE_P(FrontTests);

TYPED_TEST_P(FrontTests, WhenEmptyGetThrows) { ASSERT_THROW((void)this->emptyContainer.front(), EmptyContainerError); }

TYPED_TEST_P(FrontTests, WhenEmptyTryGetNullptr) { ASSERT_EQ(this->emptyContainer.try_front(), nullptr); }

TYPED_TEST_P(FrontTests, WhenEmptyPopThrows) { ASSERT_THROW(this->emptyContainer.pop_front(), EmptyContainerError); }

TYPED_TEST_P(FrontTests, WhenEmptyPopNThrows) { ASSERT_THROW(this->emptyContainer.pop_front(5), std::out_of_range); }

TYPED_TEST_P(FrontTests, WhenEmptyTryPopFalse) { ASSERT_FALSE(this->emptyContainer.try_pop_front()); }

TYPED_TEST_P(FrontTests, WhenEmptyTryPopNFalse) { ASSERT_FALSE(this->emptyContainer.try_pop_front(5)); }

REGISTER_TYPED_TEST_SUITE_P(FrontTests,
    WhenEmptyGetThrows,
    WhenEmptyTryGetNullptr,
    WhenEmptyPopThrows,
    WhenEmptyPopNThrows,
    WhenEmptyTryPopFalse,
    WhenEmptyTryPopNFalse
);

}