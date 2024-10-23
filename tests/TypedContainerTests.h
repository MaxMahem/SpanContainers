#pragma once

#include <cstddef>
#include <forward_list>
#include <ranges>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SpanContainerFormatter.h"

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
template <typename Adaptor>
class TypedContainerTests : public ::testing::Test 
{
protected:
    using Container  = typename Adaptor::Container;
    using PushFuncs  = typename Adaptor::PushFuncs;
    using PopFuncs   = typename Adaptor::PopFuncs;
    using PopOrder   = typename Adaptor::PopOrder;
    using IndexOrder = typename Adaptor::IndexOrder;

    std::array<int, Container::extent> emptyBuffer{};
    Container emptyContainer{ emptyBuffer };

    std::array<int, Container::extent> fullBuffer{};
    Container fullContainer{ fullBuffer };

    static constexpr std::array<int, Container::extent> FILL = []{
        std::array<int, Container::extent> result{};
        std::ranges::copy(std::views::iota(1, int{ Container::extent + 1 }), result.begin());
        return result;
    }();
    const std::forward_list<int> FILL_LIST = []{ return std::forward_list<int>(FILL.begin(), FILL.end()); }();

    static constexpr std::array<int, Container::extent> EXPECTED_RESULTS = []{
        std::array<int, Container::extent> result = FILL;
        std::ranges::sort(result, PopOrder{});
        return result;
    }();

    static constexpr std::array<int, Container::extent> INDEX_RESULTS = []{
        std::array<int, Container::extent> result = FILL;
        std::ranges::sort(result, IndexOrder{});
        return result;
    }();

    TypedContainerTests() : emptyContainer(emptyBuffer), fullContainer(fullBuffer) { }

    void SetUp() override
    {
        SCOPED_TRACE(std::format("SetUp: {}", Adaptor::NAME));

        Fill(fullContainer, FILL, PushFuncs::push);
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
   
        if constexpr (Indexable<Container>) {
            for (auto index : std::views::iota(std::size_t{ 0 }, Container::extent)) {
                EXPECT_THROW((void) container[index], std::out_of_range);
                EXPECT_EQ(container.at(index), nullptr);
            }
        }
    }

    /// @brief Tests that container is full by every avaliable measure. Also partially checks ordering
    /// @param container the container to test
    void static ExpectFull(Container container) 
    {
        EXPECT_TRUE(container.full());
        EXPECT_THAT(container, ::testing::SizeIs(Container::extent));

        if constexpr (Indexable<Container>) {
            for (auto index : std::views::iota(std::size_t{ 0 }, Container::extent)) {
                EXPECT_EQ(container[index],     INDEX_RESULTS[index]);
                EXPECT_EQ(*container.at(index), INDEX_RESULTS[index]);
            }
        }

        EXPECT_EQ(PopFuncs::get(container),      EXPECTED_RESULTS[0]);
        EXPECT_EQ(*PopFuncs::try_get(container), EXPECTED_RESULTS[0]);
    }
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
    EXPECT_FALSE(TypeParam::PushFuncs::try_push_range(this->fullContainer, TestFixture::FILL));
    EXPECT_THROW(TypeParam::PushFuncs::push_range(this->emptyContainer, TestFixture::FILL), std::out_of_range);

    // check no mutation occured
    EXPECT_THAT(this->emptyContainer, ::testing::SizeIs(partialContainerSize));
    EXPECT_EQ(TypeParam::PopFuncs::get(this->emptyContainer),      -1);
    EXPECT_EQ(*TypeParam::PopFuncs::try_get(this->emptyContainer), -1);

    // weak exception gurantee, may mutate state, but should still fail.
    EXPECT_THROW(TypeParam::PushFuncs::push_range_list(this->fullContainer, TestFixture::FILL_LIST), std::out_of_range);
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
    EXPECT_THROW(TypeParam::PopFuncs::pop_n(this->fullContainer,     TypeParam::Container::extent + 1), std::out_of_range);
    EXPECT_FALSE(TypeParam::PopFuncs::try_pop_n(this->fullContainer, TypeParam::Container::extent + 1));
    TestFixture::ExpectFull(this->fullContainer);  // also checks for mutation
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

    TestFixture::Fill(this->fullContainer, TestFixture::FILL, TypeParam::PushFuncs::push);
    while (this->fullContainer.size() > 0) { EXPECT_TRUE(TypeParam::PopFuncs::try_pop(this->fullContainer)); }
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, TestFixture::FILL, TypeParam::PushFuncs::push);
    EXPECT_NO_THROW(TypeParam::PopFuncs::pop_n(this->fullContainer, TypeParam::Container::extent));
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, TestFixture::FILL, TypeParam::PushFuncs::push);
    EXPECT_TRUE(TypeParam::PopFuncs::try_pop_n(this->fullContainer, TypeParam::Container::extent));
    TestFixture::ExpectEmpty(this->fullContainer);
}

TYPED_TEST_P(TypedContainerTests, PushPopCorrectOrder)
{
    SCOPED_TRACE("PushPopCorrectOrder");

    auto testPushPop = [&](auto push, auto&& container, std::string_view testCase) {
        ASSERT_THAT(container, ::testing::IsEmpty()) << testCase;
        this->Fill(container, TestFixture::FILL, push);
        auto values = this->Empty(container, TypeParam::PopFuncs::get, TypeParam::PopFuncs::pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(this->EXPECTED_RESULTS)) << testCase;
    };

    testPushPop(TypeParam::PushFuncs::push,     this->emptyContainer, "push");
    testPushPop(TypeParam::PushFuncs::try_push, this->emptyContainer, "try_push");

    // partially fill then clear
    std::size_t partialSize = TypeParam::Container::extent / 2;
    for (int value : TestFixture::FILL | std::views::take(partialSize)) { TypeParam::PushFuncs::push(this->emptyContainer, value); }
    this->emptyContainer.clear();

    testPushPop(TypeParam::PushFuncs::push, this->emptyContainer, "clear then push");

    // partially fill then pop empty, possibly forcing a wrap.
    for (int value : TestFixture::FILL | std::views::take(partialSize)) { TypeParam::PushFuncs::push(this->emptyContainer, value); }
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
        EXPECT_THAT(values, ::testing::ElementsAreArray(this->EXPECTED_RESULTS)) << testCase;
    };

    testPushRange(TypeParam::PushFuncs::push_range,      TestFixture::FILL,      "push_range");
    testPushRange(TypeParam::PushFuncs::try_push_range,  TestFixture::FILL,      "try_push_range");
    testPushRange(TypeParam::PushFuncs::push_range_list, TestFixture::FILL_LIST, "push_range_list");
}

TYPED_TEST_P(TypedContainerTests, IndexsAreCorrect)
{
    if constexpr (!Indexable<TypeParam::Container>) { GTEST_SKIP() << "No index methods."; }
    else {
        for (auto index = 0; index < TypeParam::Container::extent; index++) {
            EXPECT_EQ(this->fullContainer[index],     this->INDEX_RESULTS[index]);
            EXPECT_EQ(*this->fullContainer.at(index), this->INDEX_RESULTS[index]);
        }
    }
}

REGISTER_TYPED_TEST_SUITE_P(TypedContainerTests,
    ContainerFormatsCorrectly,
    ClearEmptiesContainer,
    OutOfRangePushFails,
    OutOfRangePopFails,
    PushToFullSucceeds,
    PopToEmptySucceeds,
    PushPopCorrectOrder,
    PushRangeOrderedCorrect,
    IndexsAreCorrect
);

}