#include <iterator>
#include <utility>

namespace SpanContainers {

template <typename Container>
class push_insert_iterator {
    Container* container;

public:
    using container_type    = Container;
    using value_type        = void;
    using difference_type   = std::ptrdiff_t;
    using reference         = void;
    using pointer           = void;
    using iterator_category = std::output_iterator_tag;

    explicit push_insert_iterator(Container& c) noexcept : container(&c) {}

    template <typename T>
    constexpr push_insert_iterator& operator=(T&& value)
    {
        container->push(std::forward<T>(value));  // Perfectly forward the value
        return *this;
    }

    //constexpr push_insert_iterator& operator=(const typename Container::value_type& value)
    //{
    //    container->push(value);
    //    return *this;
    //}

    //constexpr push_insert_iterator& operator=(typename Container::value_type&& value)
    //{
    //    container->push(std::move(value));
    //    return *this;
    //}

    constexpr push_insert_iterator& operator*() { return *this; }
    constexpr push_insert_iterator& operator++() { return *this; }
    constexpr push_insert_iterator operator++(int) { return *this; }
};

// Helper function to create the push_iterator
template <typename Container>
push_insert_iterator<Container> push_insert(Container& c) {
    return push_insert_iterator<Container>(c);
}

}