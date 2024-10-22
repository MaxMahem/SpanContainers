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
template <typename TestContainer>
class TypedContainerTests : public ::testing::Test 
{
protected:
    using Container  = std::tuple_element_t<0, TestContainer>;
    using PushFuncs  = std::tuple_element_t<1, TestContainer>;
    using PopFuncs   = std::tuple_element_t<2, TestContainer>;
    using IndexFuncs = std::tuple_element_t<3, TestContainer>;
    using PopOrder   = std::tuple_element_t<4, TestContainer>;

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

    TypedContainerTests() : emptyContainer(emptyBuffer), fullContainer(fullBuffer) { }

    void SetUp() override
    {
        SCOPED_TRACE("SetUp");

        for (auto value : FILL) { PushFuncs::push(fullContainer, value); }
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
   

        if constexpr (!std::is_same<IndexFuncs, NoIndex>::value) {
            for (auto index : std::views::iota(std::size_t{ 0 }, Container::extent)) {
                EXPECT_THROW((void) IndexFuncs::subscript(container, index), std::out_of_range);
                EXPECT_EQ(IndexFuncs::at(container, index), nullptr);
            }
        }
    }

    /// @brief Tests that container is full by every avaliable measure. Also partially checks ordering
    /// @param container the container to test
    void static ExpectFull(Container container) 
    {
        EXPECT_TRUE(container.full());
        EXPECT_THAT(container, ::testing::SizeIs(Container::extent));

        if constexpr (!std::is_same<IndexFuncs, NoIndex>::value) {
            for (auto index = 0; index < Container::extent; index++) {
                EXPECT_EQ(IndexFuncs::subscript(container, index), EXPECTED_RESULTS[index]);
                EXPECT_EQ(*IndexFuncs::at(container,       index), EXPECTED_RESULTS[index]);
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
        TestFixture::Container::TYPE_NAME,
        this->emptyContainer.size(),
        TestFixture::Container::extent
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
    SCOPED_TRACE("OutOfRangePushFails");

    // unconditional failure.
    EXPECT_THROW(TestFixture::PushFuncs::push(this->fullContainer, 6), FullContainerError);
    EXPECT_FALSE(TestFixture::PushFuncs::try_push(this->fullContainer, 6));
    TestFixture::ExpectFull(this->fullContainer);  // also checks for mutation

    TestFixture::PushFuncs::push(this->emptyContainer, -1);
    auto emptyBufferCopy = this->emptyBuffer;
    auto partialContainerSize = this->emptyContainer.size();

    // strong exception gurantee for failure, even if part of the push could succeed.
    EXPECT_FALSE(TestFixture::PushFuncs::try_push_range(this->fullContainer, this->FILL));
    if constexpr (TestFixture::PushFuncs::push_range_strong_exception_gurantee) {
        EXPECT_THROW(TestFixture::PushFuncs::push_range(this->emptyContainer, this->FILL), std::out_of_range);
    }

    // check no mutation occured
    EXPECT_THAT(this->emptyBuffer, ::testing::ElementsAreArray(emptyBufferCopy));
    EXPECT_THAT(this->emptyContainer, ::testing::SizeIs(partialContainerSize));
    EXPECT_EQ(TestFixture::PopFuncs::get(this->emptyContainer),      -1);
    EXPECT_EQ(*TestFixture::PopFuncs::try_get(this->emptyContainer), -1);

    // weak exception gurantee, may mutate state, but should still fail.
    EXPECT_THROW(TestFixture::PushFuncs::push_range_list(this->fullContainer, this->FILL_LIST), std::out_of_range);
    if constexpr (TestFixture::PushFuncs::push_range_strong_exception_gurantee) {
        this->emptyContainer.clear();
        this->emptyContainer.push_back(-1);
        EXPECT_THROW(TestFixture::PushFuncs::push_range(this->emptyContainer, this->FILL), std::out_of_range);
    }
}

/// @brief Tests that out of range pop fail, and validates exception gurantees.
TYPED_TEST_P(TypedContainerTests, OutOfRangePopFails)
{
    SCOPED_TRACE("OutOfRangePopFails");

    // unconditional failure.
    EXPECT_THROW(TestFixture::PopFuncs::pop(this->emptyContainer), EmptyContainerError);
    EXPECT_FALSE(TestFixture::PopFuncs::try_pop(this->emptyContainer));
    TestFixture::ExpectEmpty(this->emptyContainer);    // also checks for mutation

    // strong exception gurantee for failure, even if part of the pop could succeed.
    EXPECT_THROW(TestFixture::PopFuncs::pop_n(this->fullContainer,     TestFixture::Container::extent + 1), std::out_of_range);
    EXPECT_FALSE(TestFixture::PopFuncs::try_pop_n(this->fullContainer, TestFixture::Container::extent + 1));
    TestFixture::ExpectFull(this->fullContainer);  // also checks for mutation
}

/// @brief Tests that push methods succeed and increment size.
TYPED_TEST_P(TypedContainerTests, PushToFullSucceeds)
{
    SCOPED_TRACE("PushToFullSucceeds");

    for (int index = 0; this->emptyContainer.size() < TestFixture::Container::extent; ++index) {
        EXPECT_NO_THROW(TestFixture::PushFuncs::push(this->emptyContainer, index + 1));
    }
    TestFixture::ExpectFull(this->emptyContainer);
    
    this->emptyContainer.clear();
    for (int index = 0; this->emptyContainer.size() < TestFixture::Container::extent; ++index) {
        EXPECT_TRUE(TestFixture::PushFuncs::try_push(this->emptyContainer, index + 1)); 
    }
    TestFixture::ExpectFull(this->emptyContainer);
}

/// @brief Tests that pop methods succeed and decrement size.
TYPED_TEST_P(TypedContainerTests, PopToEmptySucceeds)
{
    SCOPED_TRACE("PopToEmptySucceeds");

    while (this->fullContainer.size() > 0) { EXPECT_NO_THROW(TestFixture::PopFuncs::pop(this->fullContainer)); }
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, this->FILL, TestFixture::PushFuncs::push);
    while (this->fullContainer.size() > 0) { EXPECT_TRUE(TestFixture::PopFuncs::try_pop(this->fullContainer)); }
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, this->FILL, TestFixture::PushFuncs::push);
    EXPECT_NO_THROW(TestFixture::PopFuncs::pop_n(this->fullContainer, TestFixture::Container::extent));
    TestFixture::ExpectEmpty(this->fullContainer);

    TestFixture::Fill(this->fullContainer, this->FILL, TestFixture::PushFuncs::push);
    EXPECT_TRUE(TestFixture::PopFuncs::try_pop_n(this->fullContainer, TestFixture::Container::extent));
    TestFixture::ExpectEmpty(this->fullContainer);
}

TYPED_TEST_P(TypedContainerTests, PushPopCorrectOrder)
{
    SCOPED_TRACE("PushPopCorrectOrder");

    auto testPushPop = [&](auto push, auto&& container, std::string_view testCase) {
        ASSERT_THAT(container, ::testing::IsEmpty()) << testCase;
        this->Fill(container, TestFixture::FILL, push);
        auto values = this->Empty(container, TestFixture::PopFuncs::get, TestFixture::PopFuncs::pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(this->EXPECTED_RESULTS)) << testCase;
    };

    testPushPop(TestFixture::PushFuncs::push,     this->emptyContainer, "push");
    testPushPop(TestFixture::PushFuncs::try_push, this->emptyContainer, "try_push");

    // partially fill then clear
    std::size_t partialSize = TestFixture::Container::extent / 2;
    for (int value : this->FILL | std::views::take(partialSize)) { TestFixture::PushFuncs::push(this->emptyContainer, value); }
    this->emptyContainer.clear();

    testPushPop(TestFixture::PushFuncs::push, this->emptyContainer, "clear then push");

    // partially fill then pop empty, possibly forcing a wrap.
    for (int value : this->FILL | std::views::take(partialSize)) { TestFixture::PushFuncs::push(this->emptyContainer, value); }
    TestFixture::PopFuncs::pop_n(this->emptyContainer, partialSize);

    testPushPop(TestFixture::PushFuncs::push, this->emptyContainer, "wrap then push");
}

TYPED_TEST_P(TypedContainerTests, PushRangeOrderedCorrect)
{
    SCOPED_TRACE("PushRangeOrderedCorrect");

    auto testPushRange = [&](auto pushRange, auto&& fill, std::string_view testCase) {
        ASSERT_THAT(this->emptyContainer, ::testing::IsEmpty()) << testCase;
        pushRange(this->emptyContainer, fill);
        auto values = this->Empty(this->emptyContainer, TestFixture::PopFuncs::get, TestFixture::PopFuncs::pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(this->EXPECTED_RESULTS)) << testCase;
    };

    testPushRange(TestFixture::PushFuncs::push_range,      this->FILL,      "push_range");
    testPushRange(TestFixture::PushFuncs::try_push_range,  this->FILL,      "try_push_range");
    testPushRange(TestFixture::PushFuncs::push_range_list, this->FILL_LIST, "push_range_list");
}

TYPED_TEST_P(TypedContainerTests, IndexsAreCorrect)
{
    if constexpr (std::is_same<TestFixture::IndexFuncs, NoIndex>::value) { GTEST_SKIP() << "No index methods."; }
    else {
        for (auto index = 0; index < TestFixture::Container::extent; index++) {
            EXPECT_EQ(TestFixture::IndexFuncs::subscript(this->fullContainer, index), this->EXPECTED_RESULTS[index]);
            EXPECT_EQ(*TestFixture::IndexFuncs::at(this->fullContainer,       index), this->EXPECTED_RESULTS[index]);
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