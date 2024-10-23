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
#include "internal/Traits/PopFrontTrait.h"


namespace SpanContainers {

/// @brief Represents a span-based, fixed size container with Queue like access.
/// @tparam T The type of the item in the container.
/// @tparam Extent The size/maximum number of elements in the container.
template <typename T, std::size_t Extent>
class SpanQueue : public internal::SpanContainer<T, Extent>,
                  public internal::IndexTrait<SpanQueue<T, Extent>, T>,
                  public internal::PushBackTrait<SpanQueue<T, Extent>, T>,
                  public internal::PopFrontTrait<SpanQueue<T, Extent>, T>
{
    friend struct internal::IndexTrait<SpanQueue<T, Extent>, T>;
    friend struct internal::PushBackTrait<SpanQueue<T, Extent>, T>;
    friend struct internal::PopFrontTrait<SpanQueue<T, Extent>, T>;

    using SpanContainer = internal::SpanContainer<T, Extent>;
    using SpanContainer::span;
    using SpanContainer::count;
    using SpanContainer::size_type;
    using SpanContainer::reference;

    /// @brief Gets a reference to the item at the front of the container without a bounds check.
    /// @return A reference to the item at the front of the container.
    [[nodiscard]] constexpr reference unsafe_front() const noexcept 
    {
        assert(count > 0 && "Container is empty");
        return span[read]; 
    }

    /// @brief Gets a reference to the element at index, without a bounds check.
    /// @param index The index of the element to get.
    /// @return A reference to the element at index.
    [[nodiscard]] constexpr reference unsafe_at(size_type index) const noexcept
    {
        assert(index < count && "Index out of range");
        return span[(read + index) % Extent];
    }

    /// @brief Assigns value to the back of the container, without bounds checks.
    /// @tparam U The type of the value.
    /// @param value The item to assign.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void unsafe_push_back(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        assert(count < Extent && "Container is full");
        span[write] = std::forward<U>(value);
        write = (write + 1) % Extent;
        ++count;
    }

    template<std::ranges::random_access_range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void unsafe_push_back_sized_range(Range&& values, size_type rangeSize) 
    {
        const auto newCount = count + rangeSize;
        assert(newCount <= Extent && "Range is to large for span.");

        const auto mid = values.begin() + (Extent - write);
        if constexpr (std::is_rvalue_reference<Range&&>::value) {
            std::ranges::move(values.begin(), mid, span.begin() + write);
            std::ranges::move(mid, values.end(), span.begin());
        }
        else {
            std::ranges::copy(values.begin(), mid, span.begin() + write);
            std::ranges::copy(mid, values.end(), span.begin());
        }
        write = (write + rangeSize) % Extent;
        count = newCount;
    }

    template<std::ranges::range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void unsafe_push_back_sized_range(Range&& values, size_type rangeSize)
    {
        assert(count + rangeSize <= Extent && "Range is to large for span.");
        for (auto&& value : values) { unsafe_push_back(std::forward<decltype(value)>(value)); }
    }

    /// @brief Remove n items from the front of the container, without bounds check.
    constexpr void unsafe_pop_front(size_type n) noexcept
    {
        assert(n <= count && "Not enough items to pop");
        count -= n;
        read = (read + n) % Extent;
    }

protected:
    /// @brief index of the read point
    SpanContainer::size_type read = 0;

    /// @brief index of the write point
    SpanContainer::size_type write = 0;

public:

    /// @brief the name of this type.
    static constexpr std::string_view TYPE_NAME = "SpanQueue";

    using SpanContainer::SpanContainer;
    using SpanContainer::operator=;

    /// @brief Clears all items from the container.
    constexpr void clear() noexcept { count = write = read = 0; }
};

}