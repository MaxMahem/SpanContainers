#pragma once
#include <type_traits>
#include <utility>

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
        if constexpr (UseExceptions) { if (asDerived().full()) { throw FullContainerError(asDerived()); } }
        asDerived().unsafe_push_front(std::forward<U>(value));
    }

    /// @brief Tries to assign value at the front of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to move to the front of the container.
    /// @return true if value was placed at the front of the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push_front(U && value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        if (asDerived().full()) { return false; }
        asDerived().unsafe_push_front(std::forward<U>(value));
        return true;
    }

    /// @brief Assign values to the front of the container.
    /// @tparam Range The type of the range that contains the values.
    /// @throws std::out_of_range If the size of the range exceeds the container capacity and UseExceptions is true.
    template<std::ranges::range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void push_front_range(Range&& values)
    {
        if constexpr (UseExceptions) {
            for (auto&& value : values) {
                if (asDerived().count + 1 > asDerived().capacity()) {
                    throw std::out_of_range(std::format("Range exceeds capacity of '{}'.", asDerived()));
                }
                asDerived().unsafe_push_front(std::forward<decltype(value)>(value));
            }
        }
        else { for (auto&& value : values) { asDerived().unsafe_push_front(std::forward<decltype(value)>(value)); } }
    }

    /// @brief Assign values to the front of the container.
    /// @details This method provides a strong exception gurantee.
    /// @tparam Range The type of the range that contains the values.
    /// @throws std::out_of_range If the size of the range exceeds the container capacity and UseExceptions is true.
    template<std::ranges::sized_range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void push_front_range(Range&& values)
    {
        const auto rangeSize = std::ranges::size(values);
        const auto newCount = asDerived().count + rangeSize;
        if constexpr (UseExceptions) { ThrowIfOutOfRange(newCount); }
        asDerived().unsafe_push_front_sized_range(std::forward<Range>(values), rangeSize);
    }

    /// @brief Tries to assign values to the front of the container.
    /// @tparam Range The type of the range that contains the values.
    /// @return `true` if values were placed at the front of the container; `false` otherwise.
    template<std::ranges::sized_range Range>
    constexpr bool try_push_front_range(Range&& values)
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    {
        const auto newCount = asDerived().count + std::ranges::size(values);
        if (newCount > asDerived().extent) { return false; }
        for (auto&& value : values) { asDerived().unsafe_push_front(std::forward<decltype(value)>(value)); }
        return true;
    }

private:
    [[nodiscard]] constexpr Derived& asDerived() noexcept { return static_cast<Derived&>(*this); }

    void ThrowIfOutOfRange(auto newCount)
    {
        if (newCount > Derived::extent) {
            throw std::out_of_range(std::format("Size of values ({}) exceeds '{}' capacity.", newCount - Derived::extent, asDerived()));
        }
    }
};

}