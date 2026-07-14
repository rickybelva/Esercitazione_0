cmake_minimum_required(VERSION 3.20)
project(Esercitazione_10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

# Cerca la libreria Eigen
find_package(Eigen3 REQUIRED)

add_executable(GradienteConiugato_test Gradienteconiugato_test.cpp)

# Collega Eigen all'eseguibile
target_link_libraries(GradienteConiugato_test Eigen3::Eigen)