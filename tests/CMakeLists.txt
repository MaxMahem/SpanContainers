cmake_minimum_required(VERSION 3.14)
project(SpanContainerTests)

include(FetchContent)

# GoogleTest
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(nameof
  URL https://github.com/Neargye/nameof/archive/refs/tags/v0.10.4.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(nameof)

enable_testing()

add_executable(SpanContainerTests
    "HeapArrayTests.cc"
    "SpanContainerTests.cc"
    "SequentialSpanContainersTests.cc"
    "SpanHeapTests.cc"
 )
target_link_libraries(SpanContainerTests GTest::gtest_main GTest::gmock_main nameof)
target_include_directories(SpanContainerTests PRIVATE "${PROJECT_INCLUDE_DIR}")


add_executable(SpanContainerDeathTests "SequentialSpanContainersDeathTests.cc")
target_link_libraries(SpanContainerDeathTests GTest::gtest_main GTest::gmock_main)
target_include_directories(SpanContainerDeathTests PRIVATE "${PROJECT_INCLUDE_DIR}")

include(GoogleTest)
gtest_discover_tests(SpanContainerTests)
gtest_discover_tests(SpanContainerDeathTests)
