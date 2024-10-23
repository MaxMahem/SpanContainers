#include <algorithm>
#include <array>
#include <cstdlib>

#include <benchmark/benchmark.h>

#include "RNG.h"

namespace SpanContainers::Benchmarks {

template <typename T, std::size_t TestSize>
class RandomDataFixture : public benchmark::Fixture {
public:
    inline static const std::array<T, TestSize> RANDOM_DATA = [] {
        std::array<T, TestSize> data{};
        std::ranges::generate(data, RNG::generate<T>);
        return data;
    }();

    inline static const std::array<T, TestSize> SORTED_DATA = [] {
        std::array<T, TestSize> data = RANDOM_DATA;
        std::ranges::sort(data);
        return data;
    }();
};

}