#pragma once

#include <array>
#include <cstddef>
#include <format>
#include <forward_list>
#include <numeric>
#include <string>
#include <string_view>

#include "SpanContainerFormatter.h"

namespace SpanContainers::Tests {

template <typename ContainerT, typename PushFuncsT, typename PopFuncsT, typename PopOrderT, typename IndexOrderT = PopOrderT>
struct ContainerTestAdaptor
{
    using Container  = ContainerT;
    using PushFuncs  = PushFuncsT;
    using PopFuncs   = PopFuncsT;
    using PopOrder   = PopOrderT;
    using IndexOrder = IndexOrderT;

    inline static const std::string NAME = std::format("{}: Push {}, Pop {}", Container::TYPE_NAME, PushFuncs::ORDER, PopFuncs::ORDER);

    static constexpr std::array<int, Container::extent> FILL = []{
        std::array<int, Container::extent> result{};
        std::iota(result.begin(), result.end(), 1);
        return result;
    }();
    inline static const std::forward_list<int> FILL_LIST = []{ return std::forward_list<int>(FILL.begin(), FILL.end()); }();

    static constexpr std::array<int, Container::extent> PUSHPOP_ORDER = [] {
        std::array<int, Container::extent> result = FILL;
        std::ranges::sort(result, PopOrder{});
        return result;
    }();

    static constexpr std::array<int, Container::extent> INDEX_ORDER = [] {
        std::array<int, Container::extent> result = FILL;
        std::ranges::sort(result, IndexOrder{});
        return result;
    }();
};

using LIFO = std::greater<int>;
using FIFO = std::less<int>;

using Ascending  = std::less<int>;
using Descending = std::greater<int>;

template <typename Container> 
struct PushBack 
{
    static constexpr std::string_view ORDER = "BACK";

    static constexpr auto push     = [](Container& container, int value) { container.push_back(value); };
    static constexpr auto try_push = [](Container& container, int value) { return container.try_push_back(value); };
    static constexpr auto push_range      = [](Container& container, std::array<int, Container::extent> values) { container.push_back_range(values); };
    static constexpr auto try_push_range  = [](Container& container, std::array<int, Container::extent> values) { return container.try_push_back_range(values); };
    static constexpr auto push_range_list = [](Container& container, std::forward_list<int> values) { container.push_back_range(values); };
};

template <typename Container> 
struct PushFront
{
    static constexpr std::string_view ORDER = "FRONT";

    static constexpr auto push     = [](Container& container, int value) { container.push_front(value); };
    static constexpr auto try_push = [](Container& container, int value) { return container.try_push_front(value); };
    static constexpr auto push_range      = [](Container& container, std::array<int, Container::extent> values) { container.push_front_range(values); };
    static constexpr auto try_push_range  = [](Container& container, std::array<int, Container::extent> values) { return container.try_push_front_range(values); };
    static constexpr auto push_range_list = [](Container& container, std::forward_list<int> values) { container.push_front_range(values); };
};

template <typename Container> 
struct PushStraight 
{
    static constexpr std::string_view ORDER = "NONE";

    static constexpr auto push     = [](Container& container, int value) { container.push(value); };
    static constexpr auto try_push = [](Container& container, int value) { return container.try_push(value); };
    static constexpr auto push_range      = [](Container& container, std::array<int, Container::extent> values) { container.push_range(values); };
    static constexpr auto try_push_range  = [](Container& container, std::array<int, Container::extent> values) { return container.try_push_range(values); };
    static constexpr auto push_range_list = [](Container& container, std::forward_list<int> values) { container.push_range(values); };
};

template <typename Container> 
struct PopBack 
{
    static constexpr std::string_view ORDER = "BACK";

    static constexpr auto get     = [](Container& container) { return container.back(); };
    static constexpr auto try_get = [](Container& container) { return container.try_back(); };
    static constexpr auto pop     = [](Container& container) { container.pop_back(); };
    static constexpr auto try_pop = [](Container& container) { return container.try_pop_back(); };
    static constexpr auto pop_n     = [](Container& container, std::size_t n) { container.pop_back(n); };
    static constexpr auto try_pop_n = [](Container& container, std::size_t n) { return container.try_pop_back(n); };
};

template <typename Container> 
struct PopFront 
{
    static constexpr std::string_view ORDER = "FRONT";

    static constexpr auto get     = [](Container& container) { return container.front(); };
    static constexpr auto try_get = [](Container& container) { return container.try_front(); };
    static constexpr auto pop     = [](Container& container) { container.pop_front(); };
    static constexpr auto try_pop = [](Container& container) { return container.try_pop_front(); };
    static constexpr auto pop_n     = [](Container& container, std::size_t n) { container.pop_front(n); };
    static constexpr auto try_pop_n = [](Container& container, std::size_t n) { return container.try_pop_front(n); };
};

template <typename Container>
concept Indexable = requires(Container container, std::size_t index) {
    { container[index] }    -> std::same_as<int&>;
    { container.at(index) } -> std::same_as<int*>;
};

}