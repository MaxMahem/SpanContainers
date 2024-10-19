#include <algorithm>
#include <array>
#include <cstdlib>

#include <benchmark/benchmark.h>

#include "RNG.h"

namespace SpanContainers::Benchmarks {

template <typename T, std::size_t TestSize>
class RandomDataFixture : public benchmark::Fixture {
public:
    std::array<T, TestSize> randomData{};

    void SetUp(const ::benchmark::State& state) override { std::ranges::generate(randomData, RNG::generate<T>); }
};

}