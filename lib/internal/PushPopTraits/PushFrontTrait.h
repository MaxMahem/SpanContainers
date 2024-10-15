#pragma once
#include <type_traits>
#include <utility>

#include "Errors/EmptyContainerError.h"
#include "Errors/FullContainerError.h"

namespace SpanContainers::internal {

template <typename Derived, typename T> struct PushFrontTrait {
    /// @brief Assigns value to the front of the container.
    /// @param value The item to place at the front of the container.
    /// @throws FullContainerError if the container capacity is exceeded
    template <typename U> constexpr void push_front(U&& value) requires std::assignable_from<T&, U&&>
    {
        auto& derived = static_cast<Derived&>(*this);
        if (!derived.try_push_front(std::forward<U>(value))) { throw FullContainerError(derived); }
    }

    /// @brief Constructs a new element in place at the front of the container from args.
    /// @tparam ...Args The type of the arguments
    /// @param ...args The arguments used to construct the element.
    /// @throws FullContainerError if the container capcity is exceeded
    template<typename... Args> requires std::is_trivially_destructible<T>::value && std::constructible_from<T, Args&&...>
    constexpr void emplace_front(Args&&... args)
    {
        auto& derived = static_cast<Derived&>(*this);
        if (!derived.try_emplace_front(std::move(args...))) { throw FullContainerError(derived); }
    }

};

}