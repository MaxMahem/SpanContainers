#pragma once

#include <cstddef>
#include <forward_list>
#include <numeric>
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
    std::array<int, Adaptor::Container::extent> buffer{};

protected:
    using Container  = typename Adaptor::Container;

    static constexpr std::size_t TEST_EXTENT = Adaptor::Container::extent;
    using TestArray = std::array<int, TEST_EXTENT>;

    static constexpr TestArray IOTA = []{
        TestArray result{};
        std::iota(result.begin(), result.end(), 0);
        return result;
    }();
    inline static const std::forward_list<int> IOTA_LIST = []{ return std::forward_list<int>(IOTA.begin(), IOTA.end()); }();

    static constexpr TestArray PUSHPOP_ORDER = []{
        TestArray result = IOTA;
        std::ranges::sort(result, typename Adaptor::PopOrder{});
        return result;
    }();

    static constexpr TestArray PUSH_ORDER = []{
        TestArray result = IOTA;
        std::ranges::sort(result, typename Adaptor::PushOrder{});
        return result;
    }();

    const Container EmptyContainer{ buffer };
    const Container FullContainer = [&]{ 
        Container fullContainer = EmptyContainer;
        Fill(fullContainer, IOTA, Adaptor::PushFuncs::push);
        return fullContainer;
    }();

    /// @brief Formats contextName with the current Adaptor::Name for SCOPE_TRACE.
    /// @param contextName The context to identify.
    /// @param contextDetail Additonal context to append.
    /// @return A string formatted with contextName and Adaptor::NAME
    [[nodiscard]] static constexpr std::string TestContext(std::string_view contextName, std::string_view contextDetail)
    { 
        return std::format("{}: {} - {}", Adaptor::NAME, contextName, contextDetail);
    }

    /// @brief Formats contextName with the current Adaptor::Name for SCOPE_TRACE.
    /// @param contextName The context to identify.
    /// @param contextDetail Additonal context to append.
    /// @return A string formatted with contextName and Adaptor::NAME
    [[nodiscard]] static constexpr std::string TestContext(std::string_view contextName)
    {
        return std::format("{}: {}", Adaptor::NAME, contextName);
    }

    /// @brief Sets up this test fixture, initializing and testing fullContainer and emptyContainer.
    void SetUp() override
    {
        SCOPED_TRACE(TestContext(NAMEOF(SetUp)));
        ExpectFull(FullContainer);
        ExpectEmpty(EmptyContainer);
    }

    /// @brief Fills container with fill via push.
    /// @param container the container to fill.
    /// @param fill The data to fill the container with.
    /// @param push the method to use to fill the container.
    [[nodiscard]] constexpr static void Fill(Container& container, auto&& fill, auto push) {
        for (auto value : fill) { push(container, value); }
        EXPECT_THAT(container, ::testing::SizeIs(std::size(fill)));
    }

    /// @brief Empties container via get and pop into a vector
    /// @param container The container to empty
    /// @param get the method to get values via
    /// @param pop the method to remove values via
    /// @return A vector filled with the data from container via get and pop.
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

        EXPECT_THROW((void) Adaptor::PopFuncs::get(container), EmptyContainerError);
        EXPECT_EQ(Adaptor::PopFuncs::try_get(container), nullptr);
    }

    /// @brief Tests that container is full by every avaliable measure. Also partially checks ordering
    /// @param container the container to test
    void static ExpectFull(Container container) 
    {
        EXPECT_TRUE(container.full());
        EXPECT_THAT(container, ::testing::SizeIs(Container::extent));

        EXPECT_EQ(Adaptor::PopFuncs::get(container),      PUSHPOP_ORDER[0]);
        EXPECT_EQ(*Adaptor::PopFuncs::try_get(container), PUSHPOP_ORDER[0]);
    }
};

}