cmake_minimum_required(VERSION 3.20)
project(esercitazione9)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")
add_executable(test_search
    test_search.cpp
)
add_executable(graph_demo
    main.cpp
)
enable_testing()
add_test(NAME search_test COMMAND test_search)