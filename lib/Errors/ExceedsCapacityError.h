#pragma once

#include <concepts>
#include <format>
#include <stdexcept>

#include "internal/FixedSizeContainer.h"

namespace SpanContainers {

/// @brief Represents an error that occurs if a change in items would exceed the capacity of a container.
class ExceedsCapacityError : public std::out_of_range 
{
public:
    /// @brief Constructs a new ExceedsCapacityError
    /// @param The message to associate with this error.
    ExceedsCapacityError(const std::string& message) : std::out_of_range(message) {}

    /// @brief Throws a ExceedsCapacityError exception if an increase of increase would exceed the capacity of container.
    /// @tparam Container A FixedSizeContainer.
    /// @param container The container to check the capcity of.
    /// @param increase The amount of additional space to check capacity for.
    template <FixedSizeContainer Container>
    static void ThrowIfExceedsCapcity(const Container& container, typename decltype(container.max_size()) increase)
    {
        if (increase + container.size() <= container.max_size()) { return; }
        throw ExceedsCapacityError(std::format("Not enough capcity in '{}' to add ({}) values.", container, increase));
    }
};

}
