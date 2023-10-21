cmake_minimum_required(VERSION 3.26)
project(static_allocation)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(static_allocation
        main.cpp)
