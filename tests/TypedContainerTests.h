#pragma once

#include <cstddef>
#include <forward_list>
#include <ranges>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SpanContainerFormatter.h"
#include "Errors/IndexOutOfRangeError.h"

#include "ContainerTestFixture.h"

namespace SpanContainers::Tests {

template <typename Container>
concept Indexable = requires(Container container, std::size_t index) {
    { container[index] }    -> std::same_as<int&>;
    { container.at(index) } -> std::same_as<int*>;
};

TYPED_TEST_SUITE_P(TypedContainerTests);

TYPED_TEST_P(TypedContainerTests, ContainerFormatsCorrectly)
{
    std::string formattedValue = std::format("{}", this->emptyContainer);
    std::string matchingRegex = std::format(
        "{}\\[{}/{}\\]",
        TypeParam::Container::TYPE_NAME,
        this->emptyContainer.size(),
        TypeParam::Container::extent
    );
    ASSERT_THAT(formattedValue, ::testing::MatchesRegex(matchingRegex));
}

/// @brief Tests that clear empties the container
TYPED_TEST_P(TypedContainerTests, ClearEmptiesContainer)
{
    SCOPED_TRACE("ClearEmptyContainer");
    this->fullContainer.clear();
    TestFixture::ExpectEmpty(this->fullContainer);
}

/// @brief Tests that out of range pushes fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerTests, OutOfRangePushFails)
{
    SCOPED_TRACE(std::format("OutOfRangePushFails: {}", TypeParam::NAME));

    // unconditional failure.
    EXPECT_THROW(TypeParam::PushFuncs::push(this->fullContainer, 6), FullContainerError);
    EXPECT_FALSE(TypeParam::PushFuncs::try_push(this->fullContainer, 6));
    TestFixture::ExpectFull(this->fullContainer);  // also checks for mutation

    TypeParam::PushFuncs::push(this->emptyContainer, -1);
    auto partialContainerSize = this->emptyContainer.size();

    // strong exception gurantee for failure, even if part of the push could succeed.
    EXPECT_FALSE(TypeParam::PushFuncs::try_push_range(this->fullContainer, TypeParam::FILL));
    EXPECT_THROW(TypeParam::PushFuncs::push_range(this->emptyContainer,    TypeParam::FILL), ExceedsCapacityError);

    // check no mutation occured
    EXPECT_THAT(this->emptyContainer, ::testing::SizeIs(partialContainerSize));
    EXPECT_EQ(TypeParam::PopFuncs::get(this->emptyContainer),      -1);
    EXPECT_EQ(*TypeParam::PopFuncs::try_get(this->emptyContainer), -1);

    // weak exception gurantee, may mutate state, but should still fail.
    EXPECT_THROW(TypeParam::PushFuncs::push_range_list(this->fullContainer, TypeParam::FILL_LIST), std::out_of_range);
}

/// @brief Tests that out of range pop fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerTests, OutOfRangePopFails)
{
    SCOPED_TRACE("OutOfRangePopFails");

    // unconditional failure.
    EXPECT_THROW(TypeParam::PopFuncs::pop(this->emptyContainer), EmptyContainerError);
    EXPECT_FALSE(TypeParam::PopFuncs::try_pop(this->emptyContainer));
    TestFixture::ExpectEmpty(this->emptyContainer);    // also checks for mutation

    // strong exception gurantee for failure, even if part of the pop could succeed.
    EXPECT_THROW(TypeParam::PopFuncs::pop_n(this->fullContainer,     TypeParam::Container::extent + 1), InsufficentItemsError);
    EXPECT_FALSE(TypeParam::PopFuncs::try_pop_n(this->fullContainer, TypeParam::Container::extent + 1));
    TestFixture::ExpectFull(this->fullContainer);  // also checks for mutation
}

TYPED_TEST_P(TypedContainerTests, OutOfRangeIndexFails)
{
    if constexpr (!Indexable<TypeParam::Container>) { GTEST_SKIP() << "No index methods."; }
    else {
        for (auto index : std::views::iota(std::size_t{ 0 }, TypeParam::Container::extent)) {
            EXPECT_THROW((void) this->emptyContainer[index], IndexOutOfRangeError);
            EXPECT_EQ(this->emptyContainer.at(index), nullptr);
        }
        EXPECT_THROW((void) this->emptyContainer[TypeParam::Container::extent + 1], IndexOutOfRangeError);
        EXPECT_EQ(this->emptyContainer.at(TypeParam::Container::extent + 1), nullptr);
    }
}

/// @brief Tests that push methods succeed and increment size.
TYPED_TEST_P(TypedContainerTests, PushToFullSucceeds)
{
    SCOPED_TRACE("PushToFullSucceeds");

    for (int index = 0; this->emptyContainer.size() < TypeParam::Container::extent; ++index) {
        EXPECT_NO_THROW(TypeParam::PushFuncs::push(this->emptyContainer, index + 1));
    }
    TestFixture::ExpectFull(this->emptyContainer);
    
    this->emptyContainer.clear();
    for (int index = 0; this->emptyContainer.size() < TypeParam::Container::extent; ++index) {
        EXPECT_TRUE(TypeParam::PushFuncs::try_push(this->emptyContainer, index + 1)); 
    }
    TestFixture::ExpectFull(this->emptyContainer);
}

/// @brief Tests that pop methods succeed and decrement size.
TYPED_TEST_P(TypedContainerTests, PopToEmptySucceeds)
{
    SCOPED_TRACE("PopToEmptySucceeds");

    while (this->fullContainer.size() > 0) { EXPECT_NO_THROW(TypeParam::PopFuncs::pop(this->fullContainer)); }
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, TypeParam::FILL, TypeParam::PushFuncs::push);
    while (this->fullContainer.size() > 0) { EXPECT_TRUE(TypeParam::PopFuncs::try_pop(this->fullContainer)); }
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, TypeParam::FILL, TypeParam::PushFuncs::push);
    EXPECT_NO_THROW(TypeParam::PopFuncs::pop_n(this->fullContainer, TypeParam::Container::extent));
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, TypeParam::FILL, TypeParam::PushFuncs::push);
    EXPECT_TRUE(TypeParam::PopFuncs::try_pop_n(this->fullContainer, TypeParam::Container::extent));
    TestFixture::ExpectEmpty(this->fullContainer);
}

TYPED_TEST_P(TypedContainerTests, PushPopCorrectOrder)
{
    SCOPED_TRACE("PushPopCorrectOrder");

    auto testPushPop = [&](auto push, auto&& container, std::string_view testCase) {
        ASSERT_THAT(container, ::testing::IsEmpty()) << testCase;
        this->Fill(container, TypeParam::FILL, push);
        auto values = this->Empty(container, TypeParam::PopFuncs::get, TypeParam::PopFuncs::pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(TypeParam::PUSHPOP_ORDER)) << testCase;
    };

    testPushPop(TypeParam::PushFuncs::push,     this->emptyContainer, "push");
    testPushPop(TypeParam::PushFuncs::try_push, this->emptyContainer, "try_push");

    // partially fill then clear
    std::size_t partialSize = TypeParam::Container::extent / 2;
    for (int value : TypeParam::FILL | std::views::take(partialSize)) { TypeParam::PushFuncs::push(this->emptyContainer, value); }
    this->emptyContainer.clear();

    testPushPop(TypeParam::PushFuncs::push, this->emptyContainer, "clear then push");

    // partially fill then pop empty, possibly forcing a wrap.
    for (int value : TypeParam::FILL | std::views::take(partialSize)) { TypeParam::PushFuncs::push(this->emptyContainer, value); }
    TypeParam::PopFuncs::pop_n(this->emptyContainer, partialSize);

    testPushPop(TypeParam::PushFuncs::push, this->emptyContainer, "wrap then push");
}

TYPED_TEST_P(TypedContainerTests, PushRangeOrderedCorrect)
{
    SCOPED_TRACE("PushRangeOrderedCorrect");

    auto testPushRange = [&](auto pushRange, auto&& fill, std::string_view testCase) {
        ASSERT_THAT(this->emptyContainer, ::testing::IsEmpty()) << testCase;
        pushRange(this->emptyContainer, fill);
        auto values = this->Empty(this->emptyContainer, TypeParam::PopFuncs::get, TypeParam::PopFuncs::pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(TypeParam::PUSHPOP_ORDER)) << testCase;
    };

    testPushRange(TypeParam::PushFuncs::push_range,      TypeParam::FILL,      "push_range");
    testPushRange(TypeParam::PushFuncs::try_push_range,  TypeParam::FILL,      "try_push_range");
    testPushRange(TypeParam::PushFuncs::push_range_list, TypeParam::FILL_LIST, "push_range_list");
}

TYPED_TEST_P(TypedContainerTests, IndexLookupCorrect)
{
    if constexpr (!Indexable<TypeParam::Container>) { GTEST_SKIP() << "No index methods."; }
    else {
        for (auto index = 0; index < TypeParam::Container::extent; index++) {
            EXPECT_EQ(this->fullContainer[index],     TypeParam::INDEX_ORDER[index]);
            EXPECT_EQ(*this->fullContainer.at(index), TypeParam::INDEX_ORDER[index]);
        }
    }
}

REGISTER_TYPED_TEST_SUITE_P(TypedContainerTests,
    ContainerFormatsCorrectly,
    ClearEmptiesContainer,
    OutOfRangePushFails,
    OutOfRangePopFails,
    OutOfRangeIndexFails,
    PushToFullSucceeds,
    PopToEmptySucceeds,
    PushPopCorrectOrder,
    PushRangeOrderedCorrect,
    IndexLookupCorrect
);

}