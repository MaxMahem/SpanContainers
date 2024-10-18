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
        if constexpr (UseExceptions) { if (asDerived().full()) { throw FullContainerError(asDerived()); } }
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

private:
    [[nodiscard]] constexpr Derived& asDerived() noexcept { return static_cast<Derived&>(*this); }
};

}