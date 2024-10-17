#include <concepts>
#include <iterator>
#include <utility>

namespace SpanContainers {

template <typename Container, typename T>
concept PushableContainer = requires(Container container, T value) {
    { container.push(std::forward<T>(value)) };
};

/// @brief Represents an output iterator that unconditionally inserts into the wrapped container via push.
/// @tparam Container The type of the wrapped container.
template <typename Container> requires PushableContainer<Container, typename Container::value_type>
class push_insert_iterator {
    Container* container;

public:
    using container_type    = Container;
    using value_type        = void;
    using difference_type   = std::ptrdiff_t;
    using reference         = void;
    using pointer           = void;
    using iterator_category = std::output_iterator_tag;

    /// @brief Creates a new push insert iterator wrapping container.
    /// @param container The container to wrap.
    explicit push_insert_iterator(Container& container) noexcept : container(&container) {}

    /// @brief Forward assigns value to the wrapped container via push.
    /// @tparam T the type of the value to assign. Must be assignable to Container::element_value.
    /// @param value The value to push.
    /// @return This iterator.
    template <typename T>
    constexpr push_insert_iterator& operator=(T&& value)
    {
        container->push(std::forward<T>(value));
        return *this;
    }

    /// @brief Does nothing.
    /// @return This iterator.
    constexpr push_insert_iterator& operator*() { return *this; }

    /// @brief Does nothing.
    /// @return This iterator.
    constexpr push_insert_iterator& operator++() { return *this; }

    /// @brief Does nothing.
    /// @return This iterator.
    constexpr push_insert_iterator operator++(int) { return *this; }
};

/// @brief Creates a push_insert_iterator wrapping container.
/// @tparam Container the type of the wraped container. Must implement push.
/// @param container the container to wrap.
/// @return a push_insert_iterator wrapping container.
template <typename Container>
push_insert_iterator<Container> push_insert(Container& container) { return push_insert_iterator<Container>(container); }

}