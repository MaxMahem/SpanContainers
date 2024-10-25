#pragma once

#include <cstddef>
#include <forward_list>
#include <ranges>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace SpanContainers::Tests {

/// @brief Test fixture for container tests
/// @tparam Adptor Adaptor type for testing.
template <typename Adaptor>
class TypedContainerTests : public ::testing::Test 
{
protected:
    using Container  = typename Adaptor::Container;
    using PushFuncs  = typename Adaptor::PushFuncs;
    using PopFuncs   = typename Adaptor::PopFuncs;

    std::array<int, Container::extent> emptyBuffer{};
    Container emptyContainer{ emptyBuffer };

    std::array<int, Container::extent> fullBuffer{};
    Container fullContainer{ fullBuffer };

    TypedContainerTests() : emptyContainer(emptyBuffer), fullContainer(fullBuffer) { }

    void SetUp() override
    {
        SCOPED_TRACE(std::format("SetUp: {}", Adaptor::NAME));

        Fill(fullContainer, Adaptor::FILL, PushFuncs::push);
        ExpectFull(fullContainer);
        ExpectEmpty(emptyContainer);
    }

    constexpr static void Fill(Container& container, auto&& fill, auto push) {
        for (auto value : fill) { push(container, value); }
    }

    [[nodiscard]] constexpr static std::vector<int> Empty(Container& container, auto get, auto pop)
    {
        std::vector<int> values{};
        while (!container.empty()) {
            values.push_back(get(container));
            pop(container);
        }
        return values;
    }

    /// @brief Tests that container is empty by every avaliable measure. Also validates failure methods for gets.
    /// @param container the container to test
    void static ExpectEmpty(Container container) 
    {
        EXPECT_THAT(container, ::testing::IsEmpty());
        EXPECT_THAT(container, ::testing::SizeIs(0));
        EXPECT_FALSE(container.full());

        EXPECT_THROW((void) PopFuncs::get(container), EmptyContainerError);
        EXPECT_EQ(PopFuncs::try_get(container), nullptr);
    }

    /// @brief Tests that container is full by every avaliable measure. Also partially checks ordering
    /// @param container the container to test
    void static ExpectFull(Container container) 
    {
        EXPECT_TRUE(container.full());
        EXPECT_THAT(container, ::testing::SizeIs(Container::extent));

        EXPECT_EQ(PopFuncs::get(container),      Adaptor::PUSHPOP_ORDER[0]);
        EXPECT_EQ(*PopFuncs::try_get(container), Adaptor::PUSHPOP_ORDER[0]);
    }
};

}