#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "internal/SpanContainer.h"

namespace SpanContainers::Tests {

using namespace SpanContainers::internal;

TEST(SpanContainerConstructors, DefaultConstructor)
{ 
    SpanContainer<int, 0> defaultContainer{};
    EXPECT_EQ(defaultContainer.capacity(), 0);
    EXPECT_THAT(defaultContainer, ::testing::SizeIs(0));
}

constexpr std::size_t TEST_EXTENT = 5;
typedef SpanContainer<int, TEST_EXTENT> TestSpanContainer;

TEST(SpanContainerConstructors, SpanAssignmentConstructor)
{
    std::array<int, TEST_EXTENT> array{};
    std::span<int, TEST_EXTENT> span = array;
    TestSpanContainer spanContainer = span;

    EXPECT_EQ(spanContainer.data().data(), span.data());
}

class SpanContainerTest : public testing::Test
{
    std::array<int, TEST_EXTENT> array{};

protected:
    TestSpanContainer container;

    SpanContainerTest() : container(array) { }
};

TEST_F(SpanContainerTest, CapacityMatchesExtent) { EXPECT_EQ(container.capacity(), TEST_EXTENT); }

TEST_F(SpanContainerTest, InitialSizeZero) { EXPECT_THAT(container, ::testing::SizeIs(0)); }

TEST_F(SpanContainerTest, InitialStateEmtpy) { EXPECT_THAT(container, ::testing::IsEmpty()); }

}