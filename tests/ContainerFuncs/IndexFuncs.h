#pragma once

#include <array>
#include <cstdarg>

namespace SpanContainers::Tests {

template <typename Container> struct IndexedFuncs {
    using IndexFunc = int(*)(const Container&, std::size_t);
    static constexpr IndexFunc at        = +[](const Container& container, std::size_t index) { return *container.at(index); };
    static constexpr IndexFunc subscript = +[](const Container& container, std::size_t index) { return container[index]; };
    static constexpr std::array<IndexFunc, 2> IndexFuncs = { at, subscript };
};

}