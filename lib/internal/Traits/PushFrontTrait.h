#pragma once
#include <type_traits>
#include <utility>

#include "Errors/ExceedsCapacityError.h"
#include "Errors/FullContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> 
struct PushFrontTrait
{
    /// @brief Assigns value to the front of the container.
    /// @param value The item to place at the front of the container.
    /// @throws FullContainerError if the container capacity is exceeded and UseExceptions is true.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void push_front(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value && !UseExceptions)
    {
        if constexpr (UseExceptions) { FullContainerError::ThrowIfFull(AsDerived()); }
        AsDerived().UnsafePushFront(std::forward<U>(value));
    }

    /// @brief Tries to assign value at the front of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to move to the front of the container.
    /// @return true if value was placed at the front of the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push_front(U && value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        if (AsDerived().full()) { return false; }
        AsDerived().UnsafePushFront(std::forward<U>(value));
        return true;
    }

    /// @brief Assign values to the front of the container.
    /// @details This method only provides the basic exception gurantee. If range exceeds the capacty of the container, 
    /// values will be pushed until capcity is reached, at which point a FullContainerError exception is thrown.
    /// @tparam Range The type of the range that contains the values.
    /// @throws std::out_of_range If the size of the range exceeds the container capacity and UseExceptions is true.
    template<std::ranges::range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void push_front_range(Range&& values) { std::ranges::copy(values, front_inserter()); }

    /// @brief Assign values to the front of the container.
    /// @details This method provides a strong exception gurantee.
    /// @tparam Range The type of the range that contains the values.
    /// @throws std::out_of_range If the size of the range exceeds the container capacity and UseExceptions is true.
    template<std::ranges::sized_range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void push_front_range(Range&& values)
    {
        const auto rangeSize = std::ranges::size(values);
        if constexpr (UseExceptions) { ExceedsCapacityError::ThrowIfExceedsCapcity(AsDerived(), rangeSize); }
        AsDerived().UnsafePushFrontRange(std::forward<Range>(values), rangeSize);
    }

    /// @brief Tries to assign values to the front of the container.
    /// @tparam Range The type of the range that contains the values.
    /// @return `true` if values were placed at the front of the container; `false` otherwise.
    template<std::ranges::sized_range Range>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr bool try_push_front_range(Range&& values)
    {
        const auto newCount = AsDerived().count + std::ranges::size(values);
        if (newCount > AsDerived().max_size()) { return false; }
        for (auto&& value : values) { AsDerived().UnsafePushFront(std::forward<decltype(value)>(value)); }
        return true;
    }

    /// @brief Gets a output iterator that inserts via push_front.
    /// @return An output iterator that inserts via push_front.
    constexpr std::front_insert_iterator<Derived> front_inserter() { return std::front_inserter(AsDerived()); }

private:
    [[nodiscard]] constexpr Derived& AsDerived() noexcept { return static_cast<Derived&>(*this); }
};

}