#include <iostream>
#include <ranges>
#include <vector>


#include "SpanDequeue.h"
#include "SpanQueue.h"
#include "SpanStack.h"
#include "SpanHeap.h"
#include "SpanContainerFormatter.h"
#include "Errors/ExceptionFormatter.h"

using namespace SpanContainers;

template <typename Container>
void demoContainer(Container& container, auto pushFunc, auto popFunc, auto getFunc)
{
    std::cout << std::format("{}\n", container);

    std::vector<int> numbers({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    for (const auto& number : numbers) {
        pushFunc(container, number);
    }

    std::cout << "Buffer after push: ";
    for (auto elem : container.data()) { std::cout << elem << " "; }
    std::cout << std::format("\nFirst item: {}\n", getFunc(container));

    try {
        pushFunc(container, 60);
    }
    catch (const FullContainerError& e) {
        std::cout << std::format("{}\n", e);
    }

    popFunc(container);
    std::cout << std::format("After pop, get item: {}\n", getFunc(container));

    pushFunc(container, 75);
    std::cout << "Buffer after pop/push 75: ";
    for (auto elem : container.data()) { std::cout << elem << " "; }
    std::cout << "\n";

    std::cout << "Pop 5: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << getFunc(container) << " ";
        popFunc(container);
    }
    std::cout << "\n";

    std::cout << std::format("Get item: {}\n\n", getFunc(container));
}

int main() {
    constexpr size_t SIZE = 10;

    {
        SpanHeap<int, SIZE>::BufferType<1> buffer{};
        SpanHeap<int, SIZE> sh{ buffer };
        std::vector<int> numbers({ 1, 32, 43, 94, 55, 76, 17, 38, 49, 10 });
        for (const auto& number : numbers) {
            sh.push(number);
        }
        std::cout << std::format("{} front: {}\n", sh, sh.back());
        sh.pop_back();
        std::cout << std::format("{} front: {}\n\n", sh, sh.back());
    }

    {
        auto buffer = SpanHeap<int, 100>::BufferType<1>();
        SpanHeap<int, 100> sh{ buffer };
        std::vector<int> numbers({ 1, 32, 43, 94, 55 });
        sh.try_push_range(numbers);
        for(int i = 0; i<9; ++i) {
            sh.try_push_range({76, 17, 38, 49, 10});
        }
        std::cout << std::format("{} front: {}\n", sh, sh.back());

        sh.try_pop_back();
        std::cout << std::format("{} front: {}\n\n", sh, sh.back());
    }

    {
        auto buffer = SpanDequeue<int, SIZE>::BufferType<1>();
        SpanDequeue<int, SIZE> sq{ buffer };
        std::vector<int> numbers({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
        for (const auto& number : numbers) {
            sq.try_push_back(number);
        }
        std::cout << std::format("{} front: {}, back: {}\n\n", sq, sq[0], sq.back());
    }

    std::cout << "SpanStack - push_back, pop_back, LIFO" << '\n';
    {
        auto buffer = SpanStack<int, SIZE>::BufferType<>();
        SpanStack<int, SIZE> ss(buffer);
        demoContainer(ss,
            [](auto& c, int val) { c.push_back(val); },
            [](auto& c) { c.pop_back(); },
            [](auto& c) { return c.back(); });
    }

    std::cout << "SpanDequeue - push_front, pop_back, FIFO" << '\n';
    {
        auto buffer = SpanDequeue<int, SIZE>::BufferType<1>();
        SpanDequeue<int, SIZE> sq(buffer);
        demoContainer(sq,
            [](auto& c, int val) { c.push_front(val); },
            [](auto& c) { c.pop_back(); },
            [](auto& c) { return c.back(); });
    }

    std::cout << "SpanDequeue - push_front, pop_front, LIFO" << '\n';
    {
        auto buffer = SpanDequeue<int, SIZE>::BufferType<>();
        SpanDequeue<int, SIZE> sq(buffer);
        demoContainer(sq,
            [](auto& c, int val) { c.push_front(val); },
            [](auto& c) { c.pop_front(); },
            [](auto& c) { return c.front(); });
    }

    std::cout << "SpanQueue - push_back, pop_front, FIFO" << '\n';
    {
        SpanQueue<int, SIZE>::BufferType<> buffer{};
        SpanQueue<int, SIZE> sq(buffer);
        demoContainer(sq,
            [](auto& c, const auto& val) { c.push_back(val); },
            [](auto& c) { c.pop_front(); },
            [](auto& c) { return c.front(); });
    }

    std::cout << "SpanHeap - push_back, pop_back, Sorted" << '\n';
    {
        SpanHeap<int, SIZE>::BufferType<> buffer{};
        SpanHeap<int, 10, std::greater<int>> sh(buffer);
        //SpanHeap<int, SIZE, std::greater<int>> sh(buffer);
        demoContainer(sh,
            [](auto& c, const auto& val) { c.push(val); },
            [](auto& c) { c.pop_back(); },
            [](auto& c) { return c.back(); });
    }

    return 0;
}

//int main()
//{
//    auto buffer = SpanDequeue<int, 10>::BufferType<>();
//    SpanDequeue<int, 10> sq(buffer);
//
//    std::cout << std::format("{}\n", sq);
//
//    std::vector<int> numbers({ 1, 2, 3, 4, 5 });
//    std::ranges::copy(numbers, std::back_inserter(sq));
//    sq.push_back(10);
//    sq.push_back(20);
//    sq.push_back(30);
//    sq.push_back(40);
//    sq.push_back(50);
//
//    std::cout << "Buffer after push_back: ";
//    for (auto elem : buffer) { std::cout << elem << " "; }
//    std::cout << std::format("\nfirst item: {}\n", sq.front());
//
//    try { sq.push_back(60); }
//    catch (const std::exception& e) {
//        std::cout << "Exception " << e.what() << '\n';
//    }
//    
//    sq.pop_front();
//    std::cout << " Buffer after pop_front: ";
//    for (auto elem : buffer) { std::cout << elem << " "; }
//    std::cout << std::format("first item: {}\n", sq.front());
//
//    sq.push_back(75);
//    std::cout << "\n" << " Buffer after push_back: ";
//    for (auto elem : buffer) { std::cout << elem << " "; }
//    std::cout << "\n";
//
//    return 0;
//}
