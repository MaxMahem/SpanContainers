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

namespace SpanContainers::Benchmarks {

template <typename T, std::size_t TestSize>
class PushPopFixture : public RandomDataFixture<T, TestSize> 
{
    using THIS = PushPopFixture<T, TestSize>;

public:

    void SpanStackArrayPushPop(benchmark::State& state) 
    {
        for (auto _ : state) {
            std::array<T, TestSize> buffer{};
            SpanStack<T, TestSize> spanStack{ buffer };

            std::ranges::copy(THIS::RANDOM_DATA, std::back_inserter(spanStack));

            auto rit = THIS::RANDOM_DATA.rbegin();
            while (!spanStack.empty()) { 
                if (spanStack.back() != *rit++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                spanStack.pop_back(); 
            }
        }
    }

    void SpanStackArrayPushPopNoMemberPtr(benchmark::State& state)
    {
        for (auto _ : state) {
            std::array<T, TestSize> buffer{};
            SpanStack<T, TestSize> spanStack{ buffer };

            for (auto&& value : THIS::RANDOM_DATA) { spanStack.push_back_no_member(value); }
            // std::ranges::copy(THIS::RANDOM_DATA, std::back_inserter(spanStack));

            auto rit = THIS::RANDOM_DATA.rbegin();
            while (!spanStack.empty()) {
                if (spanStack.back() != *rit++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                spanStack.pop_back();
            }
        }
    }

    void SpanStackHeapPushPop(benchmark::State& state) {
        for (auto _ : state) {
            SpanContainers::internal::HeapArray<T, TestSize> buffer{};
            SpanContainers::SpanStack<T, TestSize> spanStack{ buffer };

            std::ranges::copy(THIS::RANDOM_DATA, std::back_inserter(spanStack));

            auto rit = THIS::RANDOM_DATA.rbegin();
            while (!spanStack.empty()) {
                if (spanStack.back() != *rit++) {
                    state.SkipWithError("Data generated out of order!");
                    return; 
                }
                spanStack.pop_back();
            }
        }
    }

    void StdStackPushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::stack<T> stack{};

            std::ranges::copy(THIS::RANDOM_DATA, push_inserter(stack));
            
            auto rit = THIS::RANDOM_DATA.rbegin();
            while (!stack.empty()) {
                if (stack.top() != *rit++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                stack.pop();
            }
        }
    }

    void StdVectorPushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::vector<T> vector{};
            vector.reserve(TestSize);

            std::ranges::copy(THIS::RANDOM_DATA, std::back_inserter(vector));
            
            auto rit = THIS::RANDOM_DATA.rbegin();
            while (!vector.empty()) {
                if (vector.back() != *rit++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                vector.pop_back();
            }
        }
    }

    void StdPriorityQuePushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::priority_queue<T> priorityQueue{};

            std::ranges::copy(THIS::RANDOM_DATA, push_inserter(priorityQueue));

            auto rit = THIS::SORTED_DATA.rbegin();
            while (!priorityQueue.empty()) {
                if (priorityQueue.top() != *rit++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                priorityQueue.pop();
            }
        }
    }

    void HeapPushPop(benchmark::State& state) {
        for (auto _ : state) {
            std::array<T, TestSize> buffer{};
            SpanHeap<T, TestSize> heap{ buffer };

            std::ranges::copy(THIS::RANDOM_DATA, push_inserter(heap));

            auto rit = THIS::SORTED_DATA.rbegin();
            while (!heap.empty()) {
                if (heap.back() != *rit++) {
                    state.SkipWithError("Data generated out of order!");
                    return;
                }
                heap.pop_back();
            }
        }
    }
};

BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_256,  std::int64_t, 256)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_512,  std::int64_t, 512)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_1024, std::int64_t, 1024)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_2048, std::int64_t, 2048)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_256,  std::int64_t, 256)(benchmark::State& state)  { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_512,  std::int64_t, 512)(benchmark::State& state)  { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_1024, std::int64_t, 1024)(benchmark::State& state) { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdStackPushPop(state); }

BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_256,  std::int64_t, 256)(benchmark::State& state)  { this->StdVectorPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_512,  std::int64_t, 512)(benchmark::State& state)  { this->StdVectorPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_1024, std::int64_t, 1024)(benchmark::State& state) { this->StdVectorPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdVectorPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, StdPriorityQue_256, std::int64_t, 256)(benchmark::State& state) { this->StdPriorityQuePushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanHeap_256, std::int64_t, 256)(benchmark::State& state) { this->HeapPushPop(state); }

}

BENCHMARK_MAIN();