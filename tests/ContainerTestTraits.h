#pragma once

#include <array>
#include <cstddef>
#include <forward_list>

namespace SpanContainers::Tests {

template <typename Container> 
struct PushBackFuncs 
{
    static constexpr auto push     = +[](Container& container, int value) { container.push_back(value); };
    static constexpr auto try_push = +[](Container& container, int value) { return container.try_push_back(value); };
    static constexpr auto push_range      = +[](Container& container, std::array<int, Container::extent> values) { container.push_back_range(values); };
    static constexpr auto try_push_range  = +[](Container& container, std::array<int, Container::extent> values) { return container.try_push_back_range(values); };
    static constexpr auto push_range_list = +[](Container& container, std::forward_list<int> values) { container.push_back_range(values); };
    static constexpr bool push_range_strong_exception_gurantee = true;
};

template <typename Container> 
struct PushFrontFuncs 
{
    static constexpr auto push     = +[](Container& container, int value) { container.push_front(value); };
    static constexpr auto try_push = +[](Container& container, int value) { return container.try_push_front(value); };
    static constexpr auto push_range      = +[](Container& container, std::array<int, Container::extent> values) { container.push_front_range(values); };
    static constexpr auto try_push_range  = +[](Container& container, std::array<int, Container::extent> values) { return container.try_push_front_range(values); };
    static constexpr auto push_range_list = +[](Container& container, std::forward_list<int> values) { container.push_front_range(values); };
    static constexpr bool push_range_strong_exception_gurantee = false;
};

template <typename Container> 
struct PushStraightFuncs 
{
    static constexpr auto push     = +[](Container& container, int value) { container.push(value); };
    static constexpr auto try_push = +[](Container& container, int value) { return container.try_push(value); };
    static constexpr auto push_range      = +[](Container& container, std::array<int, Container::extent> values) { container.push_range(values); };
    static constexpr auto try_push_range  = +[](Container& container, std::array<int, Container::extent> values) { return container.try_push_range(values); };
    static constexpr auto push_range_list = +[](Container& container, std::forward_list<int> values) { container.push_range(values); };
    static constexpr bool push_range_strong_exception_gurantee = false;
};

template <typename Container> 
struct PopBackFuncs {
    static constexpr auto get     = +[](Container& container) { return container.back(); };
    static constexpr auto try_get = +[](Container& container) { return container.try_back(); };
    static constexpr auto pop     = +[](Container& container) { container.pop_back(); };
    static constexpr auto try_pop = +[](Container& container) { return container.try_pop_back(); };
    static constexpr auto pop_n     = +[](Container& container, std::size_t n) { container.pop_back(n); };
    static constexpr auto try_pop_n = +[](Container& container, std::size_t n) { return container.try_pop_back(n); };
};

template <typename Container> 
struct PopFrontFuncs {
    static constexpr auto get     = +[](Container& container) { return container.front(); };
    static constexpr auto try_get = +[](Container& container) { return container.try_front(); };
    static constexpr auto pop     = +[](Container& container) { container.pop_front(); };
    static constexpr auto try_pop = +[](Container& container) { return container.try_pop_front(); };
    static constexpr auto pop_n     = +[](Container& container, std::size_t n) { container.pop_front(n); };
    static constexpr auto try_pop_n = +[](Container& container, std::size_t n) { return container.try_pop_front(n); };
};

template <typename Container> 
struct IndexFuncs {
    static constexpr auto at        = +[](const Container& container, std::size_t index) { return container.at(index); };
    static constexpr auto subscript = +[](const Container& container, std::size_t index) { return container[index]; };
};

struct NoIndex {};

}