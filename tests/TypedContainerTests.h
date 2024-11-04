#pragma once

#include <cstddef>
#include <forward_list>
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

TYPED_TEST_SUITE_P(TypedContainerTests);

TYPED_TEST_P(TypedContainerTests, ContainerFormatsCorrectly)
{
    std::string formattedValue = std::format("{}", this->EmptyContainer);
    std::string matchingRegex = std::format(
        "{}\\[{}/{}\\]",
        TypeParam::Container::TYPE_NAME,
        this->EmptyContainer.size(),
        TypeParam::Container::extent
    );
    ASSERT_THAT(formattedValue, ::testing::MatchesRegex(matchingRegex));
}

/// @brief Tests that clear empties the container
TYPED_TEST_P(TypedContainerTests, ClearEmptiesContainer)
{
    SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(ClearEmptiesContainer)));
    auto clearTestContainer = this->FullContainer;
    clearTestContainer.clear();
    TestFixture::ExpectEmpty(clearTestContainer);

    // test fill after clear to validate behavior (had bug here)
    TestFixture::Fill(clearTestContainer, TestFixture::IOTA, TypeParam::PushFuncs::push);
    TestFixture::ExpectFull(clearTestContainer);
}

/// @brief Tests that out of range pushes fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerTests, OutOfRangePushFails)
{
    SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(OutOfRangePushFails)));

    // unconditional failure.
    auto fullContainer = this->FullContainer;
    EXPECT_THROW(TypeParam::PushFuncs::push(fullContainer, 6), FullContainerError);
    EXPECT_FALSE(TypeParam::PushFuncs::try_push(fullContainer, 6));
    TestFixture::ExpectFull(fullContainer);  // also checks for mutation

    auto partialyFilledContainer = this->EmptyContainer;
    TypeParam::PushFuncs::push(partialyFilledContainer, -1);
    auto partialyFilledContainerSize = partialyFilledContainer.size();

    // strong exception gurantee for failure, even if part of the push could succeed.
    EXPECT_FALSE(TypeParam::PushFuncs::try_push_range(partialyFilledContainer, TestFixture::IOTA));
    EXPECT_THROW(TypeParam::PushFuncs::push_range(partialyFilledContainer,     TestFixture::IOTA), ExceedsCapacityError);

    // check no mutation occured
    EXPECT_THAT(partialyFilledContainer, ::testing::SizeIs(partialyFilledContainerSize));
    EXPECT_EQ(TypeParam::PopFuncs::get(partialyFilledContainer),      -1);
    EXPECT_EQ(*TypeParam::PopFuncs::try_get(partialyFilledContainer), -1);

    // weak exception gurantee, may mutate state, but should still fail.
    EXPECT_THROW(TypeParam::PushFuncs::push_range_list(partialyFilledContainer, TestFixture::IOTA_LIST), std::out_of_range);
}

/// @brief Tests that out of range pop fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerTests, OutOfRangePopFails)
{
    SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(OutOfRangePopFails)));

    // unconditional failure.
    auto emptyContainer = this->EmptyContainer;
    EXPECT_THROW(TypeParam::PopFuncs::pop(emptyContainer), EmptyContainerError);
    EXPECT_FALSE(TypeParam::PopFuncs::try_pop(emptyContainer));
    TestFixture::ExpectEmpty(emptyContainer); // also checks for mutation

    // strong exception gurantee for failure, even if part of the pop could succeed.
    auto fullContainer = this->FullContainer;
    EXPECT_THROW(TypeParam::PopFuncs::pop_n(fullContainer,     TypeParam::Container::extent + 1), InsufficentItemsError);
    EXPECT_FALSE(TypeParam::PopFuncs::try_pop_n(fullContainer, TypeParam::Container::extent + 1));
    TestFixture::ExpectFull(fullContainer);  // also checks for mutation
}

/// @brief Tests that push methods succeed and increment size.
TYPED_TEST_P(TypedContainerTests, MutableGetTests)
{
    SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(MutableGetTests)));

    // tests succesfull get/try get
    auto fullContainer = this->FullContainer;
    TypeParam::PopFuncs::get(fullContainer) = 6;
    EXPECT_EQ(TypeParam::PopFuncs::get(fullContainer), 6);

    *TypeParam::PopFuncs::try_get(fullContainer) = 7;
    EXPECT_EQ(*TypeParam::PopFuncs::try_get(fullContainer), 7);

    // tests failing get/try get
    auto emptyContainer = this->EmptyContainer;
    EXPECT_THROW((void)TypeParam::PopFuncs::get(emptyContainer), EmptyContainerError);
    EXPECT_EQ(TypeParam::PopFuncs::try_get(emptyContainer), nullptr);
}

/// @brief Tests that push methods succeed and increment size.
TYPED_TEST_P(TypedContainerTests, PushToFullSucceeds)
{
    {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PushToFullSucceeds), NAMEOF(TypeParam::PushFuncs::push)));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };

        for (auto value : TestFixture::IOTA) { EXPECT_NO_THROW(TypeParam::PushFuncs::push(container, value)); }

        TestFixture::ExpectFull(container);
    }
    {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PushToFullSucceeds), NAMEOF(TypeParam::PushFuncs::try_push)));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };

        for (auto value : TestFixture::IOTA) { EXPECT_TRUE(TypeParam::PushFuncs::try_push(container, value)); }

        TestFixture::ExpectFull(container);
    }
}

/// @brief Tests that pop methods succeed and decrement size.
TYPED_TEST_P(TypedContainerTests, PopToEmptySucceeds)
{
    {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PopToEmptySucceeds), NAMEOF(TypeParam::PopFuncs::pop)));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };
        TestFixture::Fill(container, TestFixture::IOTA, TypeParam::PushFuncs::push);

        while (container.size() > 0) { EXPECT_NO_THROW(TypeParam::PopFuncs::pop(container)); }

        TestFixture::ExpectEmpty(container);
    }
    {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PopToEmptySucceeds), NAMEOF(TypeParam::PopFuncs::try_pop)));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };
        TestFixture::Fill(container, TestFixture::IOTA, TypeParam::PushFuncs::push);

        while (container.size() > 0) { EXPECT_TRUE(TypeParam::PopFuncs::try_pop(container)); }

        TestFixture::ExpectEmpty(container);
    }
    {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PopToEmptySucceeds), NAMEOF(TypeParam::PopFuncs::pop_n)));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };
        TestFixture::Fill(container, TestFixture::IOTA, TypeParam::PushFuncs::push);

        EXPECT_NO_THROW(TypeParam::PopFuncs::pop_n(container, TypeParam::Container::extent));

        TestFixture::ExpectEmpty(container);
    }
    {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PopToEmptySucceeds), NAMEOF(TypeParam::PopFuncs::try_pop_n)));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };
        TestFixture::Fill(container, TestFixture::IOTA, TypeParam::PushFuncs::push);

        EXPECT_TRUE(TypeParam::PopFuncs::try_pop_n(container, TypeParam::Container::extent));

        TestFixture::ExpectEmpty(container);
    }
}

/// @brief Tests that container fills and empties in the expected order.
TYPED_TEST_P(TypedContainerTests, PushPopCorrectOrder)
{
    auto testPushPop = [&](auto push, std::string_view testCase) {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PushPopCorrectOrder), testCase));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };

        TestFixture::Fill(container, TestFixture::IOTA, push);
        auto values = this->Empty(container, TypeParam::PopFuncs::get, TypeParam::PopFuncs::pop);

        EXPECT_THAT(values, ::testing::ElementsAreArray(TestFixture::PUSHPOP_ORDER)) << testCase;
    };

    testPushPop(TypeParam::PushFuncs::push,     NAMEOF(TypeParam::PushFuncs::push));
    testPushPop(TypeParam::PushFuncs::try_push, NAMEOF(TypeParam::PushFuncs::try_push));
}

/// @brief Tests that container fills with range and empties in expected order.
TYPED_TEST_P(TypedContainerTests, PushRangeOrderedCorrect)
{
    auto testPushRange = [&](auto pushRange, auto&& fill, std::string_view testCase) {
        SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PushRangeOrderedCorrect), testCase));
        typename TestFixture::TestArray buffer{};
        typename TestFixture::Container container{ buffer };

        pushRange(container, fill);
        auto values = this->Empty(container, TypeParam::PopFuncs::get, TypeParam::PopFuncs::pop);

        EXPECT_THAT(values, ::testing::ElementsAreArray(TestFixture::PUSHPOP_ORDER)) << testCase;
    };

    testPushRange(TypeParam::PushFuncs::push_range,      TestFixture::IOTA,      NAMEOF(TypeParam::PushFuncs::push_range));
    testPushRange(TypeParam::PushFuncs::try_push_range,  TestFixture::IOTA,      NAMEOF(TypeParam::PushFuncs::try_push_range));
    testPushRange(TypeParam::PushFuncs::push_range_list, TestFixture::IOTA_LIST, NAMEOF(TypeParam::PushFuncs::push_range_list));
}

REGISTER_TYPED_TEST_SUITE_P(TypedContainerTests,
    ContainerFormatsCorrectly,
    ClearEmptiesContainer,
    OutOfRangePushFails,
    OutOfRangePopFails,
    MutableGetTests,
    PushToFullSucceeds,
    PopToEmptySucceeds,
    PushPopCorrectOrder,
    PushRangeOrderedCorrect
);

}