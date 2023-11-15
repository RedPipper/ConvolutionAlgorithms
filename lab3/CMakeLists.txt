cmake_minimum_required(VERSION 3.24)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

set(MPI_CXX_FOUND "/usr/bin/mpic++")
find_package(MPI REQUIRED)
add_executable(lab3 main.cpp)
target_link_libraries(lab3 PRIVATE MPI::MPI_CXX)
