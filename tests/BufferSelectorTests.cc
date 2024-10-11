#include <array>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "internal\BufferSelector.h"
#include "internal\HeapArray.h"

namespace SpanContainers::Tests {

using namespace SpanContainers::internal;

constexpr std::size_t TEST_EXTENT = 10;

TEST(BufferSelectorTest, BufferSelectorStackAllocation) 
{
    using SelectedBufferType = BufferSelector<int, TEST_EXTENT>::BufferType<>;

    bool isStackAllocated = std::is_same_v<SelectedBufferType, std::array<int, TEST_EXTENT>>;

    EXPECT_TRUE(isStackAllocated);
}

TEST(BufferSelectorTest, BufferSelectorHeapAllocation) 
{
    using SelectedBufferType = BufferSelector<int, TEST_EXTENT>::BufferType<5>;

    bool isHeapAllocated = std::is_same_v<SelectedBufferType, HeapArray<int, TEST_EXTENT>>;

    EXPECT_TRUE(isHeapAllocated);
}

}