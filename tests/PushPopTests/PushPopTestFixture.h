#pragma once

#include <array>
#include <ranges>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "..\BaseTests.h"

namespace SpanContainers::Tests {

/// @brief Test fixture for testing Span container with push based insertion
/// @tparam Container the type of the container.
template <typename Container> 
class PushPopTestFixture : public BaseTests<Container> {
protected:
    using PushFunc = void(*)(Container&, int);
    using PopFunc  = void(*)(Container&);
    using PopNFunc = void(*)(Container&, int);
    using GetFunc  = int(*)(Container&);

    void TestPopDecrement(PushFunc pushMethod, PopFunc popMethod)
    {
        pushMethod(this->emptyContainer, 9);
        auto initialSize = this->emptyContainer.size();
        popMethod(this->emptyContainer);
        EXPECT_THAT(this->emptyContainer, ::testing::SizeIs(initialSize - 1));
    }

    void TestPopNDecrement(PushFunc pushMethod, PopNFunc popNMethod)
    {
        for (int value : NUMBER_FILL) { pushMethod(this->emptyContainer, value); }
        auto initialSize = this->emptyContainer.size();
        constexpr auto decrement = TEST_EXTENT - 2;

        popNMethod(this->emptyContainer, decrement);
        EXPECT_THAT(this->emptyContainer, ::testing::SizeIs(initialSize - decrement));
        this->emptyContainer.clear();
    }

    /// @brief Builds a vector by pushing Fill into container with push, then get and pops the values back into vector.
    /// @tparam The type of range to fill.
    /// @param container The SpanContainer to fill.
    /// @param fill The values to fill the container with.
    /// @param push The method to push into the container with.
    /// @param get The method to get values from the container with.
    /// @param pop The method to pop the container with.
    /// @return A vector full of values poped from the container.
    template <std::ranges::range Fill>
    std::vector<int> BuildPushPopVector(Container container, Fill fill, PushFunc push, GetFunc get, PopFunc pop)
    {
        for (int value : fill) { push(container, value); }

        std::vector<int> values;
        while (!container.empty()) { 
            values.push_back(get(container));
            pop(container);
        }
        return values;
    }
};

}