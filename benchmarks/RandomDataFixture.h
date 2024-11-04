#pragma once

#include <algorithm>
#include <array>
#include <cstdlib>
#include <numeric>

namespace SpanContainers::Benchmarks {

template <typename T, std::size_t TestSize>
struct RandomData
{
    inline static std::default_random_engine Engine{};

    inline static const std::array<T, TestSize> RANDOM_DATA = []{
        std::uniform_int_distribution<int> range{};
        std::array<T, TestSize> randomData{};
        std::ranges::generate(randomData, [&]{ return range(Engine); });
        return randomData;
    }();

    inline static const std::array<T, TestSize> IOTA_DATA = []{
        std::array<T, TestSize> iotaData{};
        std::iota(iotaData.begin(), iotaData.end(), 1);
        return iotaData;
    }();

    [[nodiscard]] static constexpr bool Validate(auto&& container) { return std::ranges::equal(container, RANDOM_DATA); }
};

template <typename T, std::size_t TestSize>
struct SortedRandomData : RandomData<T, TestSize>
{
    inline static const std::array<T, TestSize> SORTED_DATA = []{
        std::array<T, TestSize> sortedData = RandomData<T, TestSize>::RANDOM_DATA;
        std::ranges::sort(sortedData);
        return sortedData;
    }();
};

}