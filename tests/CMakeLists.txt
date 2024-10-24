cmake_minimum_required(VERSION 3.14)
project(SpanContainerTests)

include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(SpanContainerTests
    "HeapArrayTests.cc"
    "SpanContainerTests.cc"
    "SpanDequeueTests.cc"
    "SpanHeapTests.cc"
    "SpanStackTests.cc"
    "SpanQueueTests.cc"
 )
target_link_libraries(SpanContainerTests GTest::gtest_main GTest::gmock_main)
target_include_directories(SpanContainerTests PRIVATE "${PROJECT_INCLUDE_DIR}")


 add_executable(SpanContainerDeathTests
    "SpanStackDeathTests.cc"
    "SpanQueueDeathTests.cc"
    "SpanDequeueDeathTests.cc"
    "SpanHeapDeathTests.cc"
 )
target_link_libraries(SpanContainerDeathTests GTest::gtest_main GTest::gmock_main)
target_include_directories(SpanContainerDeathTests PRIVATE "${PROJECT_INCLUDE_DIR}")

include(GoogleTest)
gtest_discover_tests(SpanContainerTests)
gtest_discover_tests(SpanContainerDeathTests)
