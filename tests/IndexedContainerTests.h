#pragma once

#include <cstddef>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <nameof.hpp>

#include "SpanContainerFormatter.h"
#include "Errors/IndexOutOfRangeError.h"

#include "ContainerTestFixture.h"

namespace SpanContainers::Tests {

template <typename Adaptor>
class IndexedContainerTests : public TypedContainerTests<Adaptor> 
{
    using THIS = TypedContainerTests<Adaptor>;

protected:
    static constexpr auto ONE_LESS_IOTA_VIEW = [] { return std::views::iota(0, int{ THIS::TEST_EXTENT - 1 }); };
    static constexpr std::size_t INSERT_INDEX = THIS::TEST_EXTENT / 2;
    static constexpr int INSERT_VALUE = -1;
    static constexpr THIS::TestArray INSERT_MIDDLE = []{
        std::vector<int> temp{};
        std::ranges::copy(ONE_LESS_IOTA_VIEW(), std::back_inserter(temp));
        temp.insert(temp.begin() + INSERT_INDEX, INSERT_VALUE);

        typename THIS::TestArray result{};
        std::ranges::copy(temp, result.begin());
        return result;
    }();

    /// @brief Tests that every member in contanier matches the order in PUSH_ORDER when accessed via its index operators.
    /// @param container The container to test.
    /// @param order The order in which to test.
    /// @param testCase A test cases to append to failing output.
    static constexpr void ExpectIndexMatch(const Adaptor::Container& container, std::array<int, THIS::TEST_EXTENT> order)
    {
        for (auto index : std::views::iota(std::size_t{ 0 }, order.max_size())) {
            EXPECT_EQ(container[index],     order[index]);
            EXPECT_EQ(*container.at(index), order[index]);
        }
    }

    /// @brief Tests that every possibe member of container fails to lookup via its index operators.
    /// @param container The container to test.
    /// @param testCase A test cases to append to failing output.
    static constexpr void ExpectIndexFail(const Adaptor::Container& container)
    {
        for (auto index : std::views::iota(std::size_t{ 0 }, container.extent)) {
            EXPECT_THROW((void) container[index], IndexOutOfRangeError);
            EXPECT_EQ(container.at(index), nullptr);
        }
    }

    /// @brief Validates the state of FullContainer and EmptyContainer.
    void SetUp() override
    {
        TypedContainerTests<Adaptor>::SetUp();

        SCOPED_TRACE(this->TestContext(NAMEOF(SetUp)));
        ExpectIndexMatch(this->FullContainer, THIS::PUSH_ORDER);
        ExpectIndexFail(this->EmptyContainer);
    }
};

TYPED_TEST_SUITE_P(IndexedContainerTests);

TYPED_TEST_P(IndexedContainerTests, OutOfRangeIndexFails)
{
    EXPECT_THROW((void) this->EmptyContainer[TypeParam::Container::extent + 1], IndexOutOfRangeError);
    EXPECT_EQ(this->EmptyContainer.at(TypeParam::Container::extent + 1), nullptr);
}

TYPED_TEST_P(IndexedContainerTests, IndexMutateCorrect)
{
    auto indexMutateContainer = this->FullContainer;
    ASSERT_GE(indexMutateContainer.size(), 2);

    indexMutateContainer[0] = -1;
    EXPECT_EQ(indexMutateContainer[0], -1);

    *indexMutateContainer.at(1) = -2;
    EXPECT_EQ(*indexMutateContainer.at(1), -2);
}

/// @brief Validates that inserting from empty succeeds. This also validates that end-of-index insert succeeds.
TYPED_TEST_P(IndexedContainerTests, InsertFromEmptySucceeds)
{
    {
        SCOPED_TRACE(TestFixture::TestContext(NAMEOF_TYPE(InsertFromEmptySucceeds), "insert"));
        typename TestFixture::TestArray insertBuffer{};
        typename TestFixture::Container insertContainer{ insertBuffer };
        for (auto iota : TestFixture::IOTA) { EXPECT_NO_THROW(insertContainer.insert(insertContainer.size(), iota)); }
        TestFixture::ExpectIndexMatch(insertContainer, TestFixture::IOTA);
    }
    {
        SCOPED_TRACE(TestFixture::TestContext(NAMEOF_TYPE(InsertFromEmptySucceeds), "try_insert"));
        typename TestFixture::TestArray insertBuffer{};
        typename TestFixture::Container insertContainer{ insertBuffer };
        for (auto iota : TestFixture::IOTA) { EXPECT_NO_THROW(insertContainer.try_insert(insertContainer.size(), iota)); }
        TestFixture::ExpectIndexMatch(insertContainer, TestFixture::IOTA);
    }
}

TYPED_TEST_P(IndexedContainerTests, InsertMiddleCorrect)
{
    SCOPED_TRACE(TestFixture::TestContext(NAMEOF_TYPE(InsertMiddleCorrect)));
    auto insertContaier = this->EmptyContainer;
    std::ranges::copy(TestFixture::ONE_LESS_IOTA_VIEW(), std::back_inserter(insertContaier));
    insertContaier.insert(TestFixture::INSERT_INDEX, TestFixture::INSERT_VALUE);
    TestFixture::ExpectIndexMatch(insertContaier, TestFixture::INSERT_MIDDLE);
}

REGISTER_TYPED_TEST_SUITE_P(IndexedContainerTests, 
    OutOfRangeIndexFails, 
    IndexMutateCorrect, 
    InsertFromEmptySucceeds, 
    InsertMiddleCorrect
);

}