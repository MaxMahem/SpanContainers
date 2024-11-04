#pragma once

#include <algorithm>
#include <array>
#include <cstdlib>
#include <ranges>
#include <stack>
#include <vector>

#include <benchmark/benchmark.h>

#include "push_insert_iterator.h"

#define SC_USE_EXCEPTIONS false
#include "SpanStack.h"

#include "RandomDataFixture.h"

namespace SpanContainers::Benchmarks {

template <class ContainerAdaptor>
typename ContainerAdaptor::container_type& GetContainer(ContainerAdaptor& adaptor)
{
    struct Exposer : ContainerAdaptor {
        static typename ContainerAdaptor::container_type& Get(ContainerAdaptor& a) { return a.* & Exposer::c; }
    };
    return Exposer::Get(adaptor);
}

template <typename T, std::size_t TestSize, int TotalSteps>
class RandomWalkBench : public ::benchmark::Fixture, RandomData<T, TestSize>
{
    using THIS = RandomWalkBench;

    /// @brief A set of random steps walking within the range [0, TestSize), of at least TotalSteps in length, and 
    /// finishing at TestSize.
    inline static const std::vector<int> RANDOM_WALK = []{
        std::vector<int> steps{};
        int position = 0, distance = 0;
        std::uniform_int_distribution<int> range{ 0, TestSize };

        while (distance < TotalSteps) {
            int step = range(THIS::Engine) - position;
            steps.push_back(step);
            position += step;
            distance += std::abs(step);
        };
        steps.push_back(TestSize - position);
        return steps;
    }();

    void PerformPushPop(auto popFunc, auto pushFunc) const
    {
        std::ranges::for_each(RANDOM_WALK, [&, randomDataIt = this->RANDOM_DATA.begin()](auto step) mutable {
            if (step < 0) { popFunc(step); }
            else if (step > 0) { pushFunc(std::ranges::subrange(randomDataIt, randomDataIt + step)); }
            randomDataIt += step;
        });
    }

public:
    void SpanStack(benchmark::State& state)
    {
        for (auto _ : state) {
            std::array<T, TestSize> buffer{};
            SpanContainers::SpanStack<T, TestSize> spanStack{ buffer };

            auto popRange  = [&](int step) { spanStack.pop_back(-step); };
            auto pushRange = [&](auto range) { spanStack.push_back_range(range); };
            PerformPushPop(popRange, pushRange);
            if (!THIS::Validate(spanStack.data())) { state.SkipWithError("Failed Validation."); return; }
        }
    }

    void StdStack(benchmark::State& state)
    {
        for (auto _ : state) {
            std::stack<T> stack{};

            auto popRange  = [&](int step) { for (auto p : std::views::iota(0, -step)) { stack.pop(); }; };
            auto pushRange = [&](auto range) { std::ranges::copy(range, push_inserter(stack)); };
            PerformPushPop(popRange, pushRange);
            if (!THIS::Validate(GetContainer(stack))) { state.SkipWithError("Failed Validation."); return; }
        }
    }

    void StdVector(benchmark::State& state)
    {
        for (auto _ : state) {
            std::vector<T> vector{};
            vector.reserve(TestSize);

            auto popRange  = [&](int step) { vector.erase(vector.end() + step, vector.end()); };
            auto pushRange = [&](auto range) { std::ranges::copy(range, std::back_inserter(vector)); };
            PerformPushPop(popRange, pushRange);
            if (!THIS::Validate(vector)) { state.SkipWithError("Failed Validation."); return; }
        }

    }
};

}