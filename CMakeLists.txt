cmake_minimum_required(VERSION 3.16)

project(cpp_softmax_operator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(softmax_demo
    src/main.cpp
    src/softmax.cpp
)

target_include_directories(softmax_demo PRIVATE include)