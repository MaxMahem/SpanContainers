#pragma once

#include <cstddef>
#include <forward_list>
#include <ranges>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ContainerTestFixture.h"

namespace SpanContainers::Tests {

using namespace SpanContainers;

/// @brief Test fixture for container death tests
/// @tparam Adptor Adaptor type for testing.
template <typename Adaptor>
class TypedContainerDeathTest : public TypedContainerTests<Adaptor>
{
protected:
    TypedContainerDeathTest() { }

    void SetUp() override 
    { 
        SCOPED_TRACE(std::format("SetUp: {}", Adaptor::NAME));
        this->Fill(this->fullContainer, Adaptor::FILL, Adaptor::PushFuncs::push);
    }
};

TYPED_TEST_SUITE_P(TypedContainerDeathTest);

/// @brief Tests that out of range pushes fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerDeathTest, OutOfRangePushDeath)
{
    EXPECT_DEATH(TypeParam::PushFuncs::push(this->fullContainer, 6), "");

    TestFixture::PushFuncs::push(this->emptyContainer, -1);
    EXPECT_DEATH(TypeParam::PushFuncs::push_range(this->emptyContainer, TypeParam::FILL), "");
    this->emptyContainer.clear();

    TestFixture::PushFuncs::push(this->emptyContainer, -1);
    EXPECT_DEATH(TypeParam::PushFuncs::push_range_list(this->fullContainer, TypeParam::FILL_LIST), "");
}

/// @brief Tests that out of range pop fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerDeathTest, OutOfRangePopDeath)
{
    EXPECT_DEATH(TypeParam::PopFuncs::pop(this->emptyContainer), "");
    EXPECT_DEATH(TypeParam::PopFuncs::pop_n(this->fullContainer, TypeParam::Container::extent + 1), "");
}

TYPED_TEST_P(TypedContainerDeathTest, OutOfRangeGetDeath)
{
    EXPECT_DEATH((void) TypeParam::PopFuncs::get(this->emptyContainer), "");
}

TYPED_TEST_P(TypedContainerDeathTest, OutOfRangeIndexDeath)
{
    if constexpr (!Indexable<TypeParam::Container>) { GTEST_SKIP() << "No index methods."; }
    else {
        for (auto index = 0; index < TestFixture::Container::extent; index++) { EXPECT_DEATH((void) this->emptyContainer[index], ""); }
    }
}

REGISTER_TYPED_TEST_SUITE_P(TypedContainerDeathTest,
    OutOfRangePushDeath,
    OutOfRangePopDeath,
    OutOfRangeGetDeath,
    OutOfRangeIndexDeath
);

}