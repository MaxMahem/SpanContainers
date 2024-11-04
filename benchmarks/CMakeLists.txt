include(FetchContent)

# Google Benchmark
#FetchContent_Declare(Benchmark
#    GIT_REPOSITORY https://github.com/google/benchmark.git
#    GIT_TAG v1.9.0
#)
#FetchContent_MakeAvailable(Benchmark)

FetchContent_Declare(SpanContainerBenchmarks
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(BENCHMARK_ENABLE_TESTING OFF) # Disable building tests
FetchContent_MakeAvailable(SpanContainerBenchmarks)
add_executable(SpanContainerBenchmarks "Benchmarks.cpp")

# Link to Google Benchmark
target_link_libraries(SpanContainerBenchmarks benchmark::benchmark)
target_include_directories(SpanContainerBenchmarks PRIVATE "${PROJECT_INCLUDE_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(TARGET SpanContainerBenchmarks PROPERTY CXX_STANDARD 20)

add_custom_target(run
    COMMAND $<TARGET_FILE:SpanContainerBenchmarks> --benchmark_out=benchmarks.json
    DEPENDS SpanContainerBenchmarks
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Running SpanContainerBenchmarks with custom arguments"
)
