#pragma once
#include <array>
#include <ranges>

#include <gtest/gtest.h>

#include "../EmptyContainerTestFixture.h"

namespace SpanContainers::Tests {

/// @brief Test fixture for testing Span container with push based insertion
/// @tparam Container the type of the container.
template <typename Container, typename Inserter>
class PushTestFixture : public EmptyContainerTestFixture<Container> {
    std::array<int, 5> array{};

protected:
    Container fullContainer;

    PushTestFixture() : fullContainer(array) { std::ranges::copy(NUMBER_FILL, Inserter{ fullContainer }); }

    void SetUp() override
    {
        EmptyContainerTestFixture<Container>::SetUp();
        ASSERT_TRUE(fullContainer.full());
        ASSERT_EQ(fullContainer.size(), fullContainer.capacity());
    }

    using PushFunc = void(*)(Container&, int);
    void TestPushIncrement(Container container, PushFunc pushMethod)
    {
        auto initialSize = container.size();
        pushMethod(container, 9);
        EXPECT_EQ(initialSize + 1, container.size());
    }
};

}