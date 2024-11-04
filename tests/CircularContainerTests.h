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

#include "IndexedContainerTests.h"

namespace SpanContainers::Tests {

template <typename Adaptor>
class CircularContainerTests : public IndexedContainerTests<Adaptor>
{
    using THIS = CircularContainerTests;

protected:
    using Container = Adaptor::Container;

    static constexpr std::size_t PARTIAL_SIZE = Container::extent / 2;

    const Container EmptyWrappedContainer = [&]{
        auto wrappedContainer = this->EmptyContainer;
        WrapContainer(wrappedContainer);
        return wrappedContainer;
    }();

    constexpr void WrapContainer(Container& container) 
    {
        THIS::Fill(container, THIS::IOTA | std::views::take(PARTIAL_SIZE), Adaptor::PushFuncs::push);
        EXPECT_THAT(container, ::testing::SizeIs(PARTIAL_SIZE));

        Adaptor::PopFuncs::pop_n(container, PARTIAL_SIZE);
        THIS::ExpectEmpty(container);
    }
};

TYPED_TEST_SUITE_P(CircularContainerTests);

TYPED_TEST_P(CircularContainerTests, PushPopWithWrapCorrectOrder)
{
    SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(PushPopWithWrapCorrectOrder)));
    typename TypeParam::Container wrappedContainer = this->EmptyWrappedContainer;
    TestFixture::Fill(wrappedContainer, TestFixture::IOTA, TypeParam::PushFuncs::push);

    TestFixture::ExpectIndexMatch(wrappedContainer, TestFixture::PUSH_ORDER);

    auto values = TestFixture::Empty(wrappedContainer, TypeParam::PopFuncs::get, TypeParam::PopFuncs::pop);

    EXPECT_THAT(values, ::testing::ElementsAreArray(TestFixture::PUSHPOP_ORDER));
}

TYPED_TEST_P(CircularContainerTests, PushRangeWithWrapCorrectOrder)
{
    auto testPushRange = [&](auto pushRange, auto&& fill, std::string_view testCase) {
        SCOPED_TRACE(TestFixture::TestContext(NAMEOF_TYPE(PushRangeWithWrapCorrectOrder), testCase));
        typename TestFixture::TestArray wrappedBuffer{};
        typename TestFixture::Container wrappedContainer{ wrappedBuffer };
        TestFixture::WrapContainer(wrappedContainer);

        pushRange(wrappedContainer, fill);

        auto values = this->Empty(wrappedContainer, TypeParam::PopFuncs::get, TypeParam::PopFuncs::pop);
        EXPECT_THAT(values, ::testing::ElementsAreArray(TestFixture::PUSHPOP_ORDER));
    };

    testPushRange(TypeParam::PushFuncs::push_range,      TestFixture::IOTA,      NAMEOF(TypeParam::PushFuncs::push_range));
    testPushRange(TypeParam::PushFuncs::try_push_range,  TestFixture::IOTA,      NAMEOF(TypeParam::PushFuncs::try_push_range));
    testPushRange(TypeParam::PushFuncs::push_range_list, TestFixture::IOTA_LIST, NAMEOF(TypeParam::PushFuncs::push_range_list));
}

TYPED_TEST_P(CircularContainerTests, WrapedContainerCenterCorrect)
{
    SCOPED_TRACE(this->TestContext(NAMEOF_TYPE(WrapedContainerCenterCorrect)));
    typename TypeParam::Container wrappedContainer = this->EmptyWrappedContainer;
    TestFixture::Fill(wrappedContainer, TestFixture::IOTA, TypeParam::PushFuncs::push);

    wrappedContainer.center();

    TestFixture::ExpectIndexMatch(wrappedContainer, TestFixture::PUSH_ORDER);
}

TYPED_TEST_P(CircularContainerTests, AsSpanCorrect)
{
    typename TypeParam::Container wrappedContainer = this->EmptyWrappedContainer;
    TestFixture::Fill(wrappedContainer, TestFixture::IOTA, TypeParam::PushFuncs::push);

    EXPECT_THAT(wrappedContainer.as_span(), ::testing::ElementsAreArray(TestFixture::PUSH_ORDER));
}

REGISTER_TYPED_TEST_SUITE_P(CircularContainerTests,
    PushPopWithWrapCorrectOrder,
    PushRangeWithWrapCorrectOrder,
    WrapedContainerCenterCorrect,
    AsSpanCorrect
);

}