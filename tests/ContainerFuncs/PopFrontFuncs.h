#pragma once

#include <array>

namespace SpanContainers::Tests {

template <typename Container> struct PopFrontFuncs {
    using PopFunc = void(*)(Container&);
    static constexpr PopFunc pop        = +[](Container& container) { container.pop_front(); };
    static constexpr PopFunc try_pop    = +[](Container& container) { container.try_pop_front(); };
    static constexpr std::array<PopFunc, 2> PopFuncs = { pop, try_pop };

    using PopNFunc = void(*)(Container&, int);
    static constexpr PopNFunc pop_n        = +[](Container& container, int n) { container.pop_front(n); };
    static constexpr PopNFunc try_pop_n    = +[](Container& container, int n) { container.try_pop_front(n); };
    static constexpr std::array<PopNFunc, 2> PopNFuncs = { pop_n, try_pop_n };

    using GetFunc = int(*)(Container&);
    static constexpr GetFunc get        = +[](Container& container) { return container.front(); };
    static constexpr GetFunc try_get    = +[](Container& container) { return *container.try_front(); };
    static constexpr std::array<GetFunc, 2> GetFuncs = { get, try_get };
};

}