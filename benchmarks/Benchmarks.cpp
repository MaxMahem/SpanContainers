#include <algorithm>
#include <array>
#include <cstdlib>
#include <list>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include <benchmark/benchmark.h>

#define SPAN_CONTAINERS_USE_EXCEPTIONS false
#include "SpanStack.h"
#include "SpanHeap.h"

#include "RandomDataFixture.h"

namespace SpanContainers::Benchmarks {

using namespace SpanContainers;

template <typename T, std::size_t TestSize>
class PushPopFixture : public RandomDataFixture<T, TestSize> 
{
    using RandomDataFixture<T, TestSize>::randomData;

public:

    void SpanStackArrayPushPop(benchmark::State& state) {
        for (auto _ : state) {
            // SpanStack<T, TestSize>::BufferType<1> buffer{};
            std::array<T, TestSize> buffer{};
            SpanStack<T, TestSize> spanStack{ buffer };

            std::ranges::copy(randomData, std::back_inserter(spanStack));

            auto rit = randomData.rbegin();
            while (!spanStack.empty()) { 
                if (spanStack.back() != *rit++) { std::terminate(); }
                spanStack.pop_back(); 
            }
        }
    }

    void SpanStackHeapPushPop(benchmark::State& state) {
        for (auto _ : state) {
            SpanContainers::internal::HeapArray<T, TestSize> buffer{};
            SpanContainers::SpanStack<T, TestSize> spanStack{ buffer };

            for (T number : randomData) { spanStack.push_back(number); }

            auto rit = randomData.rbegin();
            while (!spanStack.empty()) {
                if (spanStack.back() != *rit++) { std::terminate(); }
                spanStack.pop_back();
            }
        }
    }

    void StdStackPushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::stack<T> stack{};

            for (T number : randomData) { stack.push(number); }
            
            auto rit = randomData.rbegin();
            while (!stack.empty()) {
                if (stack.top() != *rit++) { std::terminate(); }
                stack.pop();
            }
        }
    }

    void StdVectorPushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::vector<T> vector{};
            vector.reserve(TestSize);

            std::ranges::copy(randomData, std::back_inserter(vector));
            
            auto rit = randomData.rbegin();
            while (!vector.empty()) {
                if (vector.back() != *rit++) { std::terminate(); }
                vector.pop_back();
            }
        }
    }
};

BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_256,  std::int64_t, 256)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_512,  std::int64_t, 512)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_1024, std::int64_t, 1024)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_2048, std::int64_t, 2048)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_256,  int, 256)(benchmark::State& state)  { this->SpanStackHeapPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_512,  int, 512)(benchmark::State& state)  { this->SpanStackHeapPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_1024, int, 1024)(benchmark::State& state) { this->SpanStackHeapPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_2048, int, 2048)(benchmark::State& state) { this->SpanStackHeapPushPop(state); }

BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_256,  std::int64_t, 256)(benchmark::State& state)  { this->StdStackPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_512,  std::int64_t, 512)(benchmark::State& state)  { this->StdStackPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_1024, std::int64_t, 1024)(benchmark::State& state) { this->StdStackPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdStackPushPop(state); }

BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_256,  std::int64_t, 256)(benchmark::State& state)  { this->StdVectorPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_512,  std::int64_t, 512)(benchmark::State& state)  { this->StdVectorPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_1024, std::int64_t, 1024)(benchmark::State& state) { this->StdVectorPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdVectorPushPop(state); }

}

BENCHMARK_MAIN();