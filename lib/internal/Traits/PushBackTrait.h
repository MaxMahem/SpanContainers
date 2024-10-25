#pragma once
#include <type_traits>
#include <utility>

#include "Errors/FullContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> 
struct PushBackTrait
{
    /// @brief Assigns value to the back of the container.
    /// @param value The item to place at the back of the container.
    /// @throws FullContainerError if the container capacity is exceeded and UseExceptions is true.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void push_back(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value && !UseExceptions)
    {
        if constexpr (UseExceptions) { FullContainerError::ThrowIfFull(asDerived()); }
        asDerived().unsafe_push_back(std::forward<U>(value));
    }

    /// @brief Tries to assign value at the back of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to move to the back of the container.
    /// @return true if value was placed at the back of the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push_back(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        if (asDerived().full()) { return false; }
        asDerived().unsafe_push_back(std::forward<U>(value));
        return true;
    }

    /// @brief Assign values to the back of the container.
    /// @details This method only provides the basic exception gurantee. If range exceeds the capacty of the container, 
    /// values will be pushed_back until capcity is reached, at which point a FullContainerError exception is thrown.
    /// @tparam Range The type of the range that contains the values.
    /// @throws FullContainerError If the size of the range exceeds the container capacity and UseExceptions is true.
    template<std::ranges::range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void push_back_range(Range&& values)
    {
        for (auto&& value : values) { push_back(std::forward<decltype(value)>(value)); }
    }

    /// @brief Assign values to the back of the container.
    /// @details This method provides a strong exception gurantee.
    /// @tparam Range The type of the range that contains the values.
    /// @throws std::out_of_range If the size of the range exceeds the container capacity and UseExceptions is true.
    template<std::ranges::sized_range Range = std::initializer_list<T>>
        requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    constexpr void push_back_range(Range&& values)
    {
        const auto rangeSize = std::ranges::size(values);
        const auto newCount = asDerived().count + rangeSize;
        if constexpr (UseExceptions) { ThrowIfOutOfRange(newCount); }
        asDerived().unsafe_push_back_sized_range(std::forward<Range>(values), rangeSize);
    }

    /// @brief Tries to assign the values to the container.
    /// @tparam Range The type of the range that contains the values.
    /// @return `true` if values were placed at the back of the container; `false` otherwise.
    template<std::ranges::sized_range Range = std::initializer_list<T>>
    constexpr bool try_push_back_range(Range&& values)
    {
        const auto rangeSize = std::ranges::size(values);
        const auto newCount = asDerived().count + rangeSize;
        if (newCount > Derived::extent) { return false; }
        asDerived().unsafe_push_back_sized_range(std::forward<Range>(values), rangeSize);
        return true;
    }

private:
    [[nodiscard]] constexpr Derived& asDerived() noexcept { return static_cast<Derived&>(*this); }
    [[nodiscard]] constexpr const Derived& asDerived() const noexcept { return static_cast<const Derived&>(*this); }

    void ThrowIfOutOfRange(auto newCount) const
    {
        if (newCount > Derived::extent) {
            throw std::out_of_range(std::format("Size of values ({}) exceeds '{}' capacity.", newCount - Derived::extent, asDerived()));
        }
    }
};

}