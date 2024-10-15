#pragma once

#include <algorithm>
#include <cstddef>
#include <concepts>
#include <string_view>
#include <type_traits>

#include "internal/SpanContainer.h"
#include "internal/PushPopTraits/PushStraightTrait.h"
#include "internal/PushPopTraits/PopBackTrait.h"

namespace SpanContainers {

template <typename Compare, typename T>
concept StdComparer = requires(T a, T b, Compare comp) {
    { comp(a, b) } -> std::convertible_to<bool>;
};

/// @brief Represents a span-based, fixed size container with heap access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <std::swappable T, std::size_t Extent, StdComparer<T> Comparer = std::less<T>>
class SpanHeap : public internal::SpanContainer<T, Extent>,
                 public internal::PushStraightTrait<SpanHeap<T, Extent, Comparer>, T>,
                 public internal::PopBackTrait<SpanHeap<T, Extent, Comparer>, T>
{
    using SpanContainer = internal::SpanContainer<T, Extent>;

    using SpanContainer::span;
    using SpanContainer::count;

    Comparer comparer;

    /// @brief Calculates the threshold at which make should be used over push
    /// @return the smallest number of items for which push should be used over make
    auto CalculateMakeThreshold(){ return count / std::max(1, std::bit_width(count) - 1); }

public:
    using span_type = std::span<T, Extent>;
    using SpanContainer::element_type;
    using SpanContainer::size_type;
    using SpanContainer::pointer;
    using SpanContainer::const_pointer;
    using SpanContainer::reference;
    using SpanContainer::const_reference;

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanHeap";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    using SpanContainer::empty;
    using SpanContainer::full;
    using SpanContainer::capacity;
    using SpanContainer::size;

    using internal::PopBackTrait<SpanHeap<T, Extent, Comparer>, T>::try_pop_back;

    /// @brief Constructs a new SpanHeap using comparer and wrapping buffer.
    /// @tparam Buffer the type of the underlying buffer to use. Must be an lvalue able to construct a std::span 
    /// @param buffer The underlying buffer to wrap.
    /// @param comparer The comparer to use when comparing elements. Defaults to std::less.
    template <typename Buffer>
    requires std::is_lvalue_reference_v<Buffer&>&& std::is_constructible_v<span_type, Buffer&>
    constexpr SpanHeap(Buffer& buffer, Comparer comparer) noexcept : span(buffer), comparer(comparer) { }

    /// @brief Gets a pointer to the last item in the container.
    /// @return An pointer to the last item in the container or nullptr if empty.
    [[nodiscard]] constexpr pointer try_back() const noexcept { return !empty() ? &span[0] : nullptr; }

    /// @brief Tries to assign value to the back of the container.
    /// @param value The item to move to the back of the container.
    /// @return true if value was placed in the container; false if not enough room for all values.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push(U&& value) noexcept(std::is_nothrow_assignable<T, U>::value)
    {
        if (full()) { return false; }
        span[count++] = std::forward<U>(value);
        std::push_heap(span.begin(), span.begin() + count, comparer);
        return true;
    }

    /// @brief Tries to assign the values to the container.
    /// @tparam Range The type of the range that contains the values.
    /// @return true if values were placed at the back of the container; false otherwise.
    template<std::ranges::input_range Range = std::initializer_list<T>>
    constexpr bool try_push_range(Range&& values) requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    {
        const auto rangeSize = std::ranges::size(values);
        const auto newCount = count + rangeSize;
        if (newCount > Extent) { return false; }
        constexpr bool isRvalue = std::is_rvalue_reference<Range&&>::value;

        if (rangeSize > CalculateMakeThreshold()) {
            if constexpr (isRvalue) { std::ranges::move(values, span.begin() + count); }
            else                    { std::ranges::copy(values, span.begin() + count); }
            count = newCount;
            std::make_heap(span.begin(), span.begin() + count, comparer);
        }
        else {
            for (auto&& value : values) {
                if constexpr (isRvalue) { span[count++] = std::move(value); }
                else                    { span[count++] = value; }
                std::push_heap(span.begin(), span.begin() + count, comparer);
            }
        }
        return true;
    }

    /// @brief Tries to constructs a new element in place in the container from args.
    /// @tparam ...Args The type of the arguments
    /// @param ...args The arguments used to construct the element.
    /// @return true if the element was constructed in placed at the back of the container; false otherwise.
    template<typename... Args> requires std::is_trivially_destructible<T>::value && std::constructible_from<T, Args&&...>
    constexpr bool try_emplace(Args&&... args) 
    {
        if (full()) { return false; }
        new (&span[count]) T(std::forward<Args>(args)...);
        ++count;
        std::push_heap(span.begin(), span.begin() + count, comparer);
        return true;
    }

    /// @brief Tries to remove n items from the back of the container.
    /// @return true if n items were removed; false if n is greater than size.
    constexpr bool try_pop_back(size_type n)
    {
        if (n > count) { return false; }
        auto newCount = count - n;
        if (n > CalculateMakeThreshold()) {
            std::make_heap(span.begin(), span.begin() + count, comparer);
            count = newCount;
        }
        else {
            for (; n > 0; n--) { std::pop_heap(span.begin(), span.begin() + count--, comparer); }
        }
        return true;
    }

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = 0; }
};

}