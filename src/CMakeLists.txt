add_executable(Demo "Demo.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(Demo PRIVATE "${PROJECT_INCLUDE_DIR}")
