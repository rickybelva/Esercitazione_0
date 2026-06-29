cmake_minimum_required(VERSION 3.20)
project(esercitazione5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")
add_executable(measure_sorts
    measure_sorts.cpp
    timecounter.cpp
)
add_executable(test_mergesort
    test_mergesort.cpp
)
add_executable(test_quicksort
    test_quicksort.cpp
)
enable_testing()
add_test(NAME mergesort COMMAND test_mergesort)
add_test(NAME quicksort COMMAND test_quicksort)