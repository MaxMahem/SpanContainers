#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <concepts>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/Traits/PushStraightTrait.h"
#include "internal/Traits/PopBackTrait.h"

namespace SpanContainers {

template <typename Compare, typename T>
concept StdComparer = std::is_empty<Compare>::value && requires(T a, T b, Compare comp) {
    { comp(a, b) } -> std::convertible_to<bool>;
};

/// @brief Represents a span-based, fixed size container with heap access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <std::swappable T, std::size_t Extent, StdComparer<T> Comparer = std::less<T>>
class SC_EMPTY_BASES SpanHeap : public internal::SpanContainer<T, Extent>,
                                public internal::PushStraightTrait<SpanHeap<T, Extent, Comparer>, T>,
                                public internal::PopBackTrait<SpanHeap<T, Extent, Comparer>, T>
{
    friend struct internal::PushStraightTrait<SpanHeap<T, Extent, Comparer>, T>;
    friend struct internal::PopBackTrait<SpanHeap<T, Extent, Comparer>, T>;

    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanContainer::span;
    using SpanContainer::count;
    using SpanContainer::size_type;
    using SpanContainer::reference;

    SC_NO_UNIQUE_ADDRESS Comparer comparer;

    /// @brief Gets a reference to the last item in the container, without bounds check.
    /// @return A reference to the last item in the container.
    [[nodiscard]] constexpr reference unsafe_back() const noexcept 
    {
        assert(count > 0 && "Container is empty");
        return span[0]; 
    }

    /// @brief Assigns value to an element of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to assign to an element of the container.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void unsafe_push(U&& value) noexcept(std::is_nothrow_assignable<T, U>::value)
    {
        assert(count < Extent && "Container is full");
        span[count++] = std::forward<U>(value);
        std::push_heap(span.begin(), span.begin() + count, comparer);
    }

    /// @brief Removes n items from the back of the container without a bounds check.
    /// @param n The number of items to remove from the back of the container.
    constexpr void unsafe_pop_back(size_type n) noexcept
    {
        assert(n <= count && "Not enough items to pop");
        while(n-- > 0) { std::pop_heap(span.begin(), span.begin() + count--, comparer); }
    }

    /// @brief Tries to assign the values to the container.
    /// @tparam Range The type of the range that contains the values.
    template<std::ranges::sized_range Range = std::initializer_list<T>>
    constexpr void unsafe_push_sized_range(Range&& values, size_type rangeSize) requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    {
        const auto newCount = count + rangeSize;
        assert(newCount <= Extent && "Range is to large for span.");

        if (rangeSize > make_threshold()) {
            if constexpr (std::is_rvalue_reference<Range&&>::value) { std::ranges::move(values, span.begin() + count); }
            else                                                    { std::ranges::copy(values, span.begin() + count); }
            count = newCount;
            std::make_heap(span.begin(), span.begin() + count, comparer);
        }
        else {
            for (auto&& value : values) { unsafe_push(value); }
        }
    }

public:
    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanHeap";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    /// @brief Constructs a new SpanHeap using comparer and wrapping buffer.
    /// @tparam Buffer the type of the underlying buffer to use. Must be an lvalue able to construct a std::span 
    /// @param buffer The underlying buffer to wrap.
    /// @param comparer The comparer to use when comparing elements. Defaults to std::less.
    template <typename Buffer>
        requires std::is_lvalue_reference_v<Buffer&>&& std::is_constructible_v<span_type, Buffer&>
    constexpr SpanHeap(Buffer& buffer, Comparer comparer) noexcept : span(buffer), comparer(comparer) { }

    /// @brief Get the threshold at which heap_make will be used over heap_push
    /// @return the smallest number of items for which push should be used over make
    [[nodiscard]] size_type make_threshold() noexcept { return count / std::max(1, std::bit_width(count) - 1); }

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = 0; }
};

}