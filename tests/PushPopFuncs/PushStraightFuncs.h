#pragma once

#include <array>

namespace SpanContainers::Tests {

template <typename Container> struct PushStraightFuncs {
    using PushFunc = void(*)(Container&, int);
    static constexpr PushFunc push          = +[](Container& container, int value) { container.push(value); };
    static constexpr PushFunc try_push      = +[](Container& container, int value) { container.try_push(value); };
    static constexpr PushFunc emplace       = +[](Container& container, int value) { container.emplace(value); };
    static constexpr PushFunc try_emplace   = +[](Container& container, int value) { container.try_emplace(value); };
    static constexpr std::array<PushFunc, 4> PushFuncs = { push, try_push, emplace, try_emplace };
};

}