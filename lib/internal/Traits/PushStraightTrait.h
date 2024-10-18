#pragma once

#include <concepts>
#include <stdexcept>
#include <type_traits>

#include "Errors/FullContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> 
struct PushStraightTrait 
{
    /// @brief Assigns value to an element of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to place in the container.
    /// @throws FullContainerError if the container capacity is exceeded and UseExceptions is true.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr void push(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value && !UseExceptions)
    {
        if constexpr (UseExceptions) { if (asDerived().full()) { throw FullContainerError(asDerived()); } }
        asDerived().unsafe_push(std::forward<U>(value));
    }

    /// @brief Tries to assign value to an element of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to assign to an element of the container.
    /// @return true if value was assigned a value within the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        if (asDerived().full()) { return false; }
        asDerived().unsafe_push(std::forward<U>(value));
        return true;
    }

    /// @brief Assigns values to the container.
    /// @tparam Range The type of the range that contains the values.
    /// @throws std::out_of_range if the container capacity would be exceeded.
    template<std::ranges::input_range Range = std::initializer_list<T>>
    constexpr void push_range(Range&& values) requires std::convertible_to<std::ranges::range_value_t<Range>, T>
    {
        if (!static_cast<Derived&>(*this).try_push_range(std::forward(values))) 
        { 
            throw std::out_of_range("Not enough space in container.");
        }
    }

private:
    [[nodiscard]] constexpr Derived& asDerived() noexcept { return static_cast<Derived&>(*this); }
    [[nodiscard]] constexpr const Derived& asDerived() const noexcept { return static_cast<const Derived&>(*this); }
};

}