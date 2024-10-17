#pragma once

#include <concepts>
#include <type_traits>

#include "Errors/EmptyContainerError.h"
#include "Errors/FullContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> struct PushStraightTrait {
    /// @brief Assigns value to an element of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to place in the container.
    /// @throws FullContainerError if the container capacity is exceeded
    template <typename U> constexpr void push(U&& value) requires std::assignable_from<T&, U&&>
    {
        if (!try_push(std::forward<U>(value))) { throw FullContainerError(static_cast<Derived&>(*this)); }
    }

    /// @brief Tries to assign value to an element of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to assign to an element of the container.
    /// @return true if value was assigned a value within the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        auto& derived = static_cast<Derived&>(*this);
        if (derived.full()) { return false; }
        derived.unsafe_push(std::forward<U>(value));
        return true;
    }
};

}