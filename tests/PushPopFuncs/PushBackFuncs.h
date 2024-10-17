#pragma once

#include <array>

namespace SpanContainers::Tests {

template <typename Container> struct PushBackFuncs {
    using PushFunc = void(*)(Container&, int);
    static constexpr PushFunc unsafe_push = +[](Container& container, int value) { container.unsafe_push_back(value); };
    static constexpr PushFunc push        = +[](Container& container, int value) { container.push_back(value); };
    static constexpr PushFunc try_push    = +[](Container& container, int value) { container.try_push_back(value); };
    static constexpr std::array<PushFunc, 3> PushFuncs = { unsafe_push, push, try_push };
};

}