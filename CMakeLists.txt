cmake_minimum_required (VERSION 3.14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# disable Module scan for now
# set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

project ("Demo")
add_executable (Demo "src/Demo.cpp")
#target_sources(Demo PUBLIC FILE_SET CXX_MODULES)
target_sources(Demo PUBLIC)
target_include_directories(Demo PRIVATE "lib")
set_property(TARGET Demo PROPERTY CXX_STANDARD 20)

#### GTest
#find_package(GTest REQUIRED)

include(FetchContent)
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG main
# )
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(SpanContainerTests
    "tests/BufferSelectorTests.cc"
    "tests/HeapArrayTests.cc"
    "tests/SpanContainerTests.cc"
    "tests/SpanDequeueTests.cc"
    "tests/SpanHeapTests.cc"
    "tests/SpanStackTests.cc"
    "tests/SpanQueueTests.cc"
)
target_link_libraries(SpanContainerTests GTest::gtest_main GTest::gmock_main)
target_include_directories(SpanContainerTests PRIVATE "lib")

include(GoogleTest)
gtest_discover_tests(SpanContainerTests)

#### Google benchmark
#find_package(benchmark REQUIRED)
#FetchContent_Declare(googletest
#  URL https://github.com/google/benchmark/archive/refs/tags/v1.9.0.tar.gz
#  DOWNLOAD_EXTRACT_TIMESTAMP true
#)
FetchContent_Declare(Benchmarks
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
)
FetchContent_MakeAvailable(Benchmarks)
add_executable(Benchmarks "benchmarks/Benchmarks.cpp")
set_property(TARGET Benchmarks PROPERTY CXX_STANDARD 20)
target_link_libraries(Benchmarks benchmark::benchmark)
target_include_directories(Benchmarks PRIVATE "lib")