#pragma once

#include <gtest/gtest.h>

#include "Errors/EmptyContainerError.h"

#include "../BaseTests.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for testing Span containers with push back ability
/// @tparam Container the type of the container.
template <typename Container> class BackTests : public BaseTests<Container> { };

TYPED_TEST_SUITE_P(BackTests);

TYPED_TEST_P(BackTests, WhenEmptyGetThrows) { ASSERT_THROW((void) this->emptyContainer.back(), EmptyContainerError); }

TYPED_TEST_P(BackTests, WhenEmptyTryGetNullptr) { ASSERT_EQ(this->emptyContainer.try_back(), nullptr); }

TYPED_TEST_P(BackTests, WhenEmptyPopThrows) { ASSERT_THROW(this->emptyContainer.pop_back(), EmptyContainerError); }

TYPED_TEST_P(BackTests, WhenEmptyPopNThrows) { ASSERT_THROW(this->emptyContainer.pop_back(5), std::out_of_range); }

TYPED_TEST_P(BackTests, WhenEmptyTryPopFalse) { ASSERT_FALSE(this->emptyContainer.try_pop_back()); }

TYPED_TEST_P(BackTests, WhenEmptyTryPopNFalse) { ASSERT_FALSE(this->emptyContainer.try_pop_back(5)); }

REGISTER_TYPED_TEST_SUITE_P(BackTests,
    WhenEmptyGetThrows,
    WhenEmptyTryGetNullptr,
    WhenEmptyPopThrows,
    WhenEmptyPopNThrows,
    WhenEmptyTryPopFalse,
    WhenEmptyTryPopNFalse
);

}