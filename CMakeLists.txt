cmake_minimum_required(VERSION 3.14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(Demo)

# Set the include directories (shared across components)
set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib")

# Include subdirectories for demo, tests, and benchmarks
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)

#include(GoogleTest)
#gtest_discover_tests(SpanContainerTests)
