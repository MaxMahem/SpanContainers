#pragma once

#include <cstddef>
#include <forward_list>
#include <ranges>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ContainerTestTraits.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for container tests
/// @tparam TestContainer the aggregate test container type expects a tuple in the format:
/// - Container
/// - PushFuncs<Container>
/// - PopFuncs<Container>
/// - IndexFuncs<Container> - this one can also be NoIndex if no index functions.
/// - Comparer - a comparer, typically std;:less or std::greater
template <typename TestContainer>
class TypedContainerDeathTest : public ::testing::Test {

protected:
    using Container  = std::tuple_element_t<0, TestContainer>;
    using PushFuncs  = std::tuple_element_t<1, TestContainer>;
    using PopFuncs   = std::tuple_element_t<2, TestContainer>;
    using IndexFuncs = std::tuple_element_t<3, TestContainer>;

    std::array<int, Container::extent> emptyBuffer{};
    Container emptyContainer{ emptyBuffer };

    std::array<int, Container::extent> fullBuffer{};
    Container fullContainer{ fullBuffer };

    static constexpr std::array<int, Container::extent> FILL = []{
        std::array<int, Container::extent> result{};
        std::ranges::copy(std::views::iota(1, int{ Container::extent + 1 }), result.begin());
        return result;
    }();
    const std::forward_list<int> FILL_LIST = [] { return std::forward_list<int>(FILL.begin(), FILL.end()); }();

    TypedContainerDeathTest() : emptyContainer(emptyBuffer), fullContainer(fullBuffer) { }

    void SetUp() override { for (auto value : FILL) { PushFuncs::push(fullContainer, value); } }
};

TYPED_TEST_SUITE_P(TypedContainerDeathTest);

/// @brief Tests that out of range pushes fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerDeathTest, OutOfRangePushDeath)
{
    EXPECT_DEATH(TestFixture::PushFuncs::push(this->fullContainer, 6), "");

    TestFixture::PushFuncs::push(this->emptyContainer, -1);
    EXPECT_DEATH(TestFixture::PushFuncs::push_range(this->emptyContainer, this->FILL), "");

    TestFixture::PushFuncs::push(this->emptyContainer, -1);
    EXPECT_DEATH(TestFixture::PushFuncs::push_range_list(this->fullContainer, this->FILL_LIST), "");
}

/// @brief Tests that out of range pop fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerDeathTest, OutOfRangePopDeath)
{
    EXPECT_DEATH(TestFixture::PopFuncs::pop(this->emptyContainer), "");
    EXPECT_DEATH(TestFixture::PopFuncs::pop_n(this->fullContainer, TestFixture::Container::extent + 1), "");
}

TYPED_TEST_P(TypedContainerDeathTest, OutOfRangeGetDeath)
{
    EXPECT_DEATH(TestFixture::PopFuncs::get(this->emptyContainer), "");
}

TYPED_TEST_P(TypedContainerDeathTest, OutOfRangeIndexDeath)
{
    if constexpr (std::is_same<TestFixture::IndexFuncs, NoIndex>::value) { GTEST_SKIP() << "No index methods."; }
    else {
        for (auto index = 0; index < TestFixture::Container::extent; index++) {
            EXPECT_DEATH(TestFixture::IndexFuncs::subscript(this->emptyContainer, index), "");
        }
    }
}

REGISTER_TYPED_TEST_SUITE_P(TypedContainerDeathTest,
    OutOfRangePushDeath,
    OutOfRangePopDeath,
    OutOfRangeGetDeath,
    OutOfRangeIndexDeath
);

}