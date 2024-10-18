add_executable(Demo "Demo.cpp")

target_include_directories(Demo PRIVATE "${PROJECT_INCLUDE_DIR}")
set_property(TARGET Demo PROPERTY CXX_STANDARD 20)