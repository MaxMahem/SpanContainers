#include <array>

#include <gtest/gtest.h>

#include "internal\BufferSelector.h"
#include "internal\HeapArray.h"

namespace SpanContainers::Tests {

constexpr std::size_t TEST_EXTENT = 10;

TEST(BufferSelectorTest, BufferSelectorStackAllocation) 
{
    using SelectedBufferType = internal::BufferSelector<int, TEST_EXTENT>::BufferType<>;
    bool isStackAllocated = std::is_same<SelectedBufferType, std::array<int, TEST_EXTENT>>::value;
    EXPECT_TRUE(isStackAllocated);
}

TEST(BufferSelectorTest, BufferSelectorHeapAllocation) 
{
    using SelectedBufferType = internal::BufferSelector<int, TEST_EXTENT>::BufferType<5>;
    bool isHeapAllocated = std::is_same<SelectedBufferType, internal::HeapArray<int, TEST_EXTENT>>::value;
    EXPECT_TRUE(isHeapAllocated);
}

}