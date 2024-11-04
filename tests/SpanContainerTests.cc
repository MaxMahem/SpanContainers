#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "internal/SpanContainer.h"

namespace SpanContainers::Tests {

using namespace SpanContainers::internal;

TEST(SpanContainerConstructors, DefaultConstructor)
{ 
    SpanContainer<int, 0> defaultContainer{};
    EXPECT_EQ(defaultContainer.max_size(), 0);
    EXPECT_THAT(defaultContainer, ::testing::SizeIs(0));
}

constexpr std::size_t TEST_EXTENT = 5;
typedef SpanContainer<int, TEST_EXTENT> TestSpanContainer;

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

}