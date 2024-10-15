#pragma once

#include <concepts>
#include <type_traits>

#include "Errors/EmptyContainerError.h"
#include "Errors/FullContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> struct PushStraightTrait {
    /// @brief Assigns value to the container.
    /// @param value The item to place at the back of the container.
    /// @throws FullContainerError if the container capacity is exceeded
    template <typename U> constexpr void push(U&& value) requires std::assignable_from<T&, U&&>
    {
        auto& derived = static_cast<Derived&>(*this);
        if (!derived.try_push(std::forward<U>(value))) { throw FullContainerError(derived); }
    }

    /// @brief Constructs a new element in place in the container from args.
    /// @tparam ...Args The type of the arguments
    /// @param ...args The arguments used to construct the element.
    /// @throws FullContainerError if the container capcity is exceeded
    template<typename... Args> requires std::is_trivially_destructible<T>::value && std::constructible_from<T, Args&&...>
    constexpr void emplace(Args&&... args)
    {
        auto& derived = static_cast<Derived&>(*this);
        if (!derived.try_emplace(std::forward<Args...>(args...))) { throw FullContainerError(derived); }
    }
};

}