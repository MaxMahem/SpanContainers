#include <concepts>
#include <limits>
#include <random>

namespace SpanContainers::Benchmarks {

class RNG {
    static std::mt19937& engine() {
        static std::random_device device;
        static std::mt19937 engine{ device() };
        return engine;
    }

public:
    template <std::integral T> static T generate() {
        static std::uniform_int_distribution<T> distribution{ std::numeric_limits<T>::min(), std::numeric_limits<T>::max() };
        return distribution(engine());
    }

    template <std::floating_point T> static T generate() {
        static std::uniform_real_distribution<T> distribution{ 0.0, 1.0 };
        return distribution(engine());
    }
};

}