cmake_minimum_required(VERSION 3.16)

project(cpp_softmax_operator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(softmax_demo
    src/main.cpp
    src/softmax.cpp
)

target_include_directories(softmax_demo PRIVATE include)

add_executable(test_driver
    tests/test_driver.cpp
    src/softmax.cpp
)

target_include_directories(test_driver PRIVATE include)

enable_testing()
find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_test(NAME softmax_1d
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tests/test_softmax.py
)

set_tests_properties(softmax_1d PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)