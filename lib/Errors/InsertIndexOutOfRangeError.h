#pragma once

#include <format>
#include <string>

#include "internal/FixedSizeContainer.h"
#include "IndexOutOfRangeError.h"

namespace SpanContainers {

/// @brief Represents an error that occurs if a given index is out of range for insertion into a container.
class InsertIndexOutOfRangeError : public IndexOutOfRangeError
{
public:
    /// @brief Constructs a new InsertIndexOutOfRangeError
    /// @param The message to associate with this error.
    InsertIndexOutOfRangeError(const std::string& message) : IndexOutOfRangeError(message) {}

    /// @brief Throws a InsertIndexOutOfRangeError if index is out of range for insertion into a container.
    /// @tparam Container A FixedSizeContainer.
    /// @param container The container to check the range of index on.
    /// @param index The index to check.
    template <FixedSizeContainer Container>
    static void ThrowIfOutOfRange(const Container& container, typename decltype(container.size()) index)
    {
        if (index <= container.size()) { return; }
        throw InsertIndexOutOfRangeError(std::format("Index ({}) is out of range for '{}'.", index, container));
    }
};

}
