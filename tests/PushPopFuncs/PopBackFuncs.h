#pragma once

#include <array>

namespace SpanContainers::Tests {

template <typename Container> struct PopBackFuncs {
    using PopFunc = void(*)(Container&);
    static constexpr PopFunc pop     = +[](Container& container) { container.pop_back(); };
    static constexpr PopFunc try_pop = +[](Container& container) { container.try_pop_back(); };
    static constexpr std::array<PopFunc, 2> PopFuncs = { pop, try_pop };

    using PopNFunc = void(*)(Container&, int);
    static constexpr PopNFunc pop_n     = +[](Container& container, int n) { container.pop_back(n); };
    static constexpr PopNFunc try_pop_n = +[](Container& container, int n) { container.try_pop_back(n); };

    using GetFunc = int(*)(Container&);
    static constexpr GetFunc get     = +[](Container& container) { return container.back(); };
    static constexpr GetFunc try_get = +[](Container& container) { return *container.try_back(); };
    static constexpr std::array<GetFunc, 2> GetFuncs = { get, try_get };
};

}