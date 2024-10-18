#pragma once

#include <array>

namespace SpanContainers::Tests {

template <typename Container> struct PopFrontFuncs {
    using PopFunc = void(*)(Container&);
    static constexpr PopFunc unsafe_pop = +[](Container& container) { container.unsafe_pop_front(); };
    static constexpr PopFunc pop        = +[](Container& container) { container.pop_front(); };
    static constexpr PopFunc try_pop    = +[](Container& container) { container.try_pop_front(); };
    static constexpr std::array<PopFunc, 3> PopFuncs = { unsafe_pop, pop, try_pop };

    using PopNFunc = void(*)(Container&, int);
    static constexpr PopNFunc unsafe_pop_n = +[](Container& container, int n) { container.unsafe_pop_front(n); };
    static constexpr PopNFunc pop_n        = +[](Container& container, int n) { container.pop_front(n); };
    static constexpr PopNFunc try_pop_n    = +[](Container& container, int n) { container.try_pop_front(n); };
    static constexpr std::array<PopNFunc, 3> PopNFuncs = { unsafe_pop_n, pop_n, try_pop_n };

    using GetFunc = int(*)(Container&);
    static constexpr GetFunc unsafe_get = +[](Container& container) { return container.unsafe_front(); };
    static constexpr GetFunc get        = +[](Container& container) { return container.front(); };
    static constexpr GetFunc try_get    = +[](Container& container) { return *container.try_front(); };
    static constexpr std::array<GetFunc, 3> GetFuncs = { unsafe_get, get, try_get };
};

}