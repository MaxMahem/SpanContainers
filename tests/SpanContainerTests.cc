#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "internal/SpanContainer.h"

namespace SpanContainers::Tests {

using namespace SpanContainers::internal;

// Static Tests

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

TEST(BufferSelectorTest, BufferSelectorStackAllocation)
{
    using SelectedBufferType = internal::SpanContainer<int, TEST_EXTENT>::BufferType<>;
    bool isStackAllocated = std::is_same<SelectedBufferType, std::array<int, TEST_EXTENT>>::value;
    ASSERT_TRUE(isStackAllocated);
}

TEST(BufferSelectorTest, BufferSelectorHeapAllocation)
{
    using SelectedBufferType = internal::SpanContainer<int, TEST_EXTENT>::BufferType<TEST_EXTENT>;
    bool isHeapAllocated = std::is_same<SelectedBufferType, internal::HeapArray<int, TEST_EXTENT>>::value;
    ASSERT_TRUE(isHeapAllocated);
}

// instance tests

class SpanContainerTest : public testing::Test
{
    std::array<int, TEST_EXTENT> array{};

protected:
    TestSpanContainer container;

    SpanContainerTest() : container(array) { }
};

TEST_F(SpanContainerTest, CapacityMatchesExtent) { EXPECT_EQ(container.capacity(), TEST_EXTENT); }

TEST_F(SpanContainerTest, InitialStateEmtpy) 
{
    EXPECT_THAT(container, ::testing::SizeIs(0));
    EXPECT_THAT(container, ::testing::IsEmpty()); 
}

}