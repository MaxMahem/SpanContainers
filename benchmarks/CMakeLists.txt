include(FetchContent)

# Google Benchmark
#FetchContent_Declare(Benchmark
#    GIT_REPOSITORY https://github.com/google/benchmark.git
#    GIT_TAG v1.9.0
#)
#FetchContent_MakeAvailable(Benchmark)

FetchContent_Declare(Benchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(Benchmark)
add_executable(Benchmarks "Benchmarks.cpp")

# Link to Google Benchmark
target_link_libraries(Benchmarks benchmark::benchmark)
target_include_directories(Benchmarks PRIVATE "${PROJECT_INCLUDE_DIR}")

set_property(TARGET Benchmarks PROPERTY CXX_STANDARD 20)
