cmake_minimum_required(VERSION 3.16)

project(cpp_softmax_operator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenMP REQUIRED)

add_executable(softmax_demo
    src/main.cpp
    src/softmax.cpp
)

target_include_directories(softmax_demo PRIVATE include)
target_link_libraries(softmax_demo PRIVATE OpenMP::OpenMP_CXX)

add_executable(test_driver
    tests/test_driver.cpp
    src/softmax.cpp
)

target_include_directories(test_driver PRIVATE include)
target_link_libraries(test_driver PRIVATE OpenMP::OpenMP_CXX)

add_executable(test_optimization
    tests/test_optimization.cpp
    src/softmax.cpp
)

target_include_directories(test_optimization PRIVATE include)
target_link_libraries(test_optimization PRIVATE OpenMP::OpenMP_CXX)

add_executable(softmax_benchmark
    benchmarks/benchmark_softmax.cpp
    src/softmax.cpp
)

target_include_directories(softmax_benchmark PRIVATE include)
target_link_libraries(softmax_benchmark PRIVATE OpenMP::OpenMP_CXX)

enable_testing()
find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_test(NAME softmax_1d
    COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tests/test_softmax_1d.py
)

add_test(NAME softmax_2d
    COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/tests/test_softmax_2d.py
)

add_test(NAME softmax_optimization
    COMMAND test_optimization
)

set_tests_properties(
    softmax_1d
    softmax_2d
    softmax_optimization
    PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RUN_SERIAL TRUE
)