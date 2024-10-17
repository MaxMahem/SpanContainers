#pragma once
#include <type_traits>
#include <utility>

#include "Errors/FullContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> struct PushBackTrait {
    /// @brief Assigns value to the back of the container.
    /// @param value The item to place at the back of the container.
    /// @throws FullContainerError if the container capacity is exceeded
    template <typename U> constexpr void push_back(U&& value) requires std::assignable_from<T&, U&&>
    {
        if (!try_push_back(std::forward<U>(value))) { throw FullContainerError(static_cast<Derived&>(*this)); }
    }

    /// @brief Tries to assign value at the back of the container.
    /// @tparam U the type of the value to assign to the container. Must be assignable to T.
    /// @param value The item to move to the back of the container.
    /// @return true if value was placed at the back of the container; false otherwise.
    template <typename U> requires std::assignable_from<T&, U&&>
    constexpr bool try_push_back(U&& value) noexcept(std::is_nothrow_assignable<T&, U&&>::value)
    {
        auto& derived = static_cast<Derived&>(*this);
        if (derived.full()) { return false; }
        derived.unsafe_push_back(std::forward<U>(value));
        return true;
    }
};

}