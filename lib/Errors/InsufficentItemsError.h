#pragma once

#include <concepts>
#include <format>
#include <stdexcept>

#include "internal/FixedSizeContainer.h"

namespace SpanContainers {

/// @brief Represents an error that occurs if there would not be enough items for a change.
class InsufficentItemsError : public std::out_of_range
{
public:
    /// @brief Constructs a new InsufficentItemsError
    /// @param The message to associate with this error.
    InsufficentItemsError(const std::string& message = "Insufficent items for change.") : std::out_of_range(message) {}

    /// @brief Throws a InsufficentItemsError exception if container does not have enough items to remove decrease items.
    /// @tparam Container The type of the container, a formattable span container.
    /// @tparam Integer An integer
    /// @param container The container to check.
    /// @param decrease The count of items to check if there is capacity to remove.
    template <FixedSizeContainer Container>
    static void ThrowIfInsufficentItems(const Container& container, typename decltype(container.size()) decrease)
    {
        if (decrease <= container.size()) { return; }
        throw InsufficentItemsError(std::format("Not enough items in '{}' to remove ({}) items.", container, decrease));
    }
};

}
