cmake_minimum_required(VERSION 3.13)
project(3_2_os)

set(CMAKE_CXX_STANDARD 14)

add_executable(3_2_os
        phil.cpp)
include_directories(.)

target_link_libraries(3_2_os pthread)

