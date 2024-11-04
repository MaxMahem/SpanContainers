#include <algorithm>
#include <array>
#include <cstdlib>
#include <list>
#include <queue>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include <benchmark/benchmark.h>

#include "push_insert_iterator.h"

#define SC_USE_EXCEPTIONS false
#include "SpanStack.h"
#include "SpanHeap.h"

#include "RandomDataFixture.h"
#include "RandomWalkBenchmarks.h"

namespace SpanContainers::Benchmarks {

template <typename T, std::size_t TestSize>
class HeapBenchmarks : public ::benchmark::Fixture, SortedRandomData<T, TestSize>
{
    using THIS = HeapBenchmarks<T, TestSize>;

public:
    void StdPriorityQuePushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::priority_queue<T> priority_queue{};

            std::ranges::copy(THIS::RANDOM_DATA, push_inserter(priority_queue));

            auto randomDataReverseIterator = THIS::SORTED_DATA.rbegin();
            while (!priority_queue.empty()) {
                if (priority_queue.top() != *randomDataReverseIterator++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                priority_queue.pop();
            }
        }
    }

    void HeapPushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::array<T, TestSize> buffer{};
            SpanHeap<T, TestSize> heap{ buffer };

            std::ranges::copy(THIS::RANDOM_DATA, push_inserter(heap));

            auto randomDataReverseIterator = THIS::SORTED_DATA.rbegin();
            while (!heap.empty()) {
                if (heap.back() != *randomDataReverseIterator++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                heap.pop_back();
            }
        }
    }
};

BENCHMARK_TEMPLATE_F(RandomWalkBench, SpanStack_256, std::int64_t, 256, 20000)(benchmark::State& state) { this->SpanStack(state); }
BENCHMARK_TEMPLATE_F(RandomWalkBench, StdStack_256,  std::int64_t, 256, 20000)(benchmark::State& state) { this->StdStack(state); }
BENCHMARK_TEMPLATE_F(RandomWalkBench, StdVector_256, std::int64_t, 256, 20000)(benchmark::State& state) { this->StdVector(state); }

//BENCHMARK_TEMPLATE_F(PushPopBenchmarks, SpanStack_256,  std::int64_t, 256)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_512,  std::int64_t, 512)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_1024, std::int64_t, 1024)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_2048, std::int64_t, 2048)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_256,  std::int64_t, 256)(benchmark::State& state)  { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_512,  std::int64_t, 512)(benchmark::State& state)  { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_1024, std::int64_t, 1024)(benchmark::State& state) { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdStackPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_256,  std::int64_t, 256)(benchmark::State& state)  { this->StdVectorPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_512,  std::int64_t, 512)(benchmark::State& state)  { this->StdVectorPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_1024, std::int64_t, 1024)(benchmark::State& state) { this->StdVectorPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdVectorPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, StdPriorandomDataReverseIteratoryQue_256, std::int64_t, 256)(benchmark::State& state) { this->StdPriorityQuePushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanHeap_256, std::int64_t, 256)(benchmark::State& state) { this->HeapPushPop(state); }

}

BENCHMARK_MAIN();