cmake_minimum_required(VERSION 3.20)
project(range_minimum_query LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

FetchContent_Declare(googletest
	GIT_REPOSITORY 	https://github.com/google/googletest.git
	GIT_TAG		v1.18.0
	GIT_SHALLOW	TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(benchmark
	GIT_REPOSITORY	https://github.com/google/benchmark.git	
	GIT_TAG		v1.10.0
	GIT_SHALLOW	TRUE
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest benchmark)

add_library(range_minimum_query
	src/sparse_table.cpp
	src/segment_tree.cpp
	src/fischer_heun.cpp
	src/naive.cpp
	src/linear.cpp
)

target_include_directories(range_minimum_query PUBLIC include)
target_compile_options(range_minimum_query PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
include(GoogleTest)

add_executable(test_common tests/test_common.cpp)
target_link_libraries(test_common PRIVATE range_minimum_query GTest::gtest_main)
gtest_discover_tests(test_common)

add_executable(range_minimum_query_bench bench/bench_main.cpp)
target_link_libraries(range_minimum_query_bench PRIVATE range_minimum_query benchmark::benchmark)
target_compile_options(range_minimum_query_bench PRIVATE -O3 -march=native)
