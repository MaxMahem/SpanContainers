#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ranges>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/Traits/IndexTrait.h"
#include "internal/Traits/PushBackTrait.h"
#include "internal/Traits/PopBackTrait.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with stack/vector like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SC_EMPTY_BASES SpanVector : public SpanStack<T, Extent>
{
    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanContainer::span;
    using SpanContainer::count;
    using SpanContainer::size_type;
    using SpanContainer::reference;

public:
    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanVector";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using value_type = T;


    void UnsafeInsert(size_type index, auto&& item) 
    {
        assert(index < count  && "Index out of range");
        assert(count < Extent && "Container is full");

        std::move_backward(span.begin() + index, span.end() - 1, span.end());
        span[index] = std::forward<T>(item);
    }
};

}