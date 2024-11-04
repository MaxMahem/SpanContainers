#pragma once

#include <cstddef>

namespace SpanContainers::internal {

/// @brief Represents a integer index for a circular buffer within a fixed range of [0, Extent), and wrapping behavior.
/// @tparam Extent The maximum size of this index.
template<std::size_t Extent>
class CircularIndex {
    size_t index = 0;

public:
    /// @brief Creates a new index initialized to index.
    /// @param index The value to initialize this index to.
    constexpr explicit CircularIndex(std::size_t index) : index(index) { assert(index < Extent && "Index out of range"); }

    /// @brief Assigns index to this value
    /// @param index the index value to assign.
    /// @return This after the assigment.
    constexpr CircularIndex& operator=(std::size_t index) 
    {
        assert(index < Extent && "Index out of range");
        this->index = index;
        return *this;
    }

    /// @brief Implictily converts this index into a std::size_t.
    [[nodiscard]] constexpr operator std::size_t() const noexcept { return index; }

    /// @brief Gets the value of this index.
    /// @return The current value of this index.
    [[nodiscard]] constexpr std::size_t value() const noexcept { return index; }

    /// @brief Pre-increments this index, wrapping if necessary.
    /// @return This index, incrmented by one.
    constexpr CircularIndex& operator++() noexcept
    {
        index = index + 1 == Extent ? 0 : index + 1;
        return *this;
    }

    /// @brief Post-increments this index, wrapping if necessary.
    /// @return This index before increment.
    [[nodiscard]] constexpr CircularIndex operator++(int) noexcept
    {
        CircularIndex temp = *this;
        ++(*this);
        return temp;
    }

    /// @brief Pre-decrements this index, wrapping if necessary.
    /// @return This index, decremented by one.
    constexpr CircularIndex& operator--() noexcept
    {
        index = index == 0 ? Extent - 1 : index - 1;
        return *this;
    }

    /// @brief Post-decrements this index, wrapping if necessary.
    /// @return This index before decrement.
    [[nodiscard]] constexpr CircularIndex operator--(int) noexcept
    {
        CircularIndex temp = *this;
        --(*this);
        return temp;
    }

    /// @brief Returns the index previous to this value, wrapping if necessary.
    /// @return The index previous to this one.
    [[nodiscard]] constexpr CircularIndex previous() const noexcept
    {
        return CircularIndex{ index == 0 ? Extent - 1 : index - 1 };
    }

    /// @brief Returns the index after this value, wrapping if necessary.
    /// @return The index after this one.
    [[nodiscard]] constexpr CircularIndex after() const noexcept
    {
        return CircularIndex{ index + 1 == Extent ? 0 : index + 1 };
    }

    /// @brief Adds offset to this index, wrapping if necessary.
    /// @param offset The offset to add.
    /// @return A new index, offset by offset.
    [[nodiscard]] constexpr CircularIndex operator+(std::size_t offset) const noexcept {
        return CircularIndex((index + offset) % Extent);
    }

    /// @brief Subtracts offset from this index, wrapping if necessary.
    /// @param offset The offset to subtract.
    /// @return A new index, offset by the subtraction of offset.
    [[nodiscard]] constexpr CircularIndex operator-(std::size_t offset) const noexcept {
        return CircularIndex((index + Extent - (offset % Extent)) % Extent);
    }

    auto operator<=>(const CircularIndex&) const = default;
};

}
