#include <algorithm>
#include <array>
#include <cstdlib>
#include <list>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include <benchmark/benchmark.h>

#include "SpanStack.h"
#include "SpanHeap.h"
#include "SpanContainerFormatter.h"

using namespace SpanContainers;

class RNG {
    static std::mt19937& engine() {
        static std::random_device device;
        static std::mt19937 engine{ device() };
        return engine;
    }

public:
    template <std::integral T> static T generate() {
        static std::uniform_int_distribution<T> distribution{ std::numeric_limits<T>::min(), std::numeric_limits<T>::max() };
        return distribution(engine());
    }

    template <std::floating_point T> static T generate() {
        static std::uniform_real_distribution<T> distribution{ 0.0, 1.0 };
        return distribution(engine());
    }

    template <typename T> static T generate() requires requires { typename T::first_type; typename T::second_type; } {
        return { generate<typename T::first_type>(), generate<typename T::second_type>() };
    }
};

template <typename T, std::size_t TestSize> 
class RandomDataFixture : public benchmark::Fixture {
public:
    std::array<T, TestSize> randomData{};

    void SetUp(const ::benchmark::State& state) override { std::ranges::generate(randomData, RNG::generate<T>); }
};

template <typename T, std::size_t TestSize>
class PushPopFixture : public RandomDataFixture<T, TestSize> {
public:
    using RandomDataFixture<T, TestSize>::randomData;

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

    void SpanStackArrayUnsafePushPop(benchmark::State& state) {
        for (auto _ : state) {
            // SpanStack<T, TestSize>::BufferType<1> buffer{};
            std::array<T, TestSize> buffer{};
            SpanStack<T, TestSize> spanStack{ buffer };

            for (T number : randomData) { spanStack.unsafe_push_back(number); }

            auto rit = randomData.rbegin();
            while (!spanStack.empty()) {
                if (spanStack.unsafe_back() != *rit++) { std::terminate(); }
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

//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_256,  int, 256)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_512,  int, 512)(benchmark::State& state)  { this->SpanStackArrayPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_1024, int, 1024)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Array_2048, std::int64_t, 2048)(benchmark::State& state) { this->SpanStackArrayPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_ArrayUnsafe_256,  int, 256)(benchmark::State& state)  { this->SpanStackArrayUnsafePushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_ArrayUnsafe_512,  int, 512)(benchmark::State& state)  { this->SpanStackArrayUnsafePushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_ArrayUnsafe_1024, int, 1024)(benchmark::State& state) { this->SpanStackArrayUnsafePushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_ArrayUnsafe_2048, std::int64_t, 2048)(benchmark::State& state) { this->SpanStackArrayUnsafePushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_256,  int, 256)(benchmark::State& state)  { this->SpanStackHeapPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_512,  int, 512)(benchmark::State& state)  { this->SpanStackHeapPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_1024, int, 1024)(benchmark::State& state) { this->SpanStackHeapPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, SpanStack_Heap_2048, int, 2048)(benchmark::State& state) { this->SpanStackHeapPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_256,  int, 256)(benchmark::State& state)  { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_512,  int, 512)(benchmark::State& state)  { this->StdStackPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_1024, int, 1024)(benchmark::State& state) { this->StdStackPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Stack_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdStackPushPop(state); }

//BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_256,  int, 256)(benchmark::State& state)  { this->StdVectorPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_512,  int, 512)(benchmark::State& state)  { this->StdVectorPushPop(state); }
//BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_1024, int, 1024)(benchmark::State& state) { this->StdVectorPushPop(state); }
BENCHMARK_TEMPLATE_F(PushPopFixture, Vector_2048, std::int64_t, 2048)(benchmark::State& state) { this->StdVectorPushPop(state); }

BENCHMARK_MAIN();