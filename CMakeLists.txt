cmake_minimum_required(VERSION 3.20)
project(lab2)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab2 main.cpp function.hpp copy.cpp Memory.hpp CacheL2.hpp Cache_Line.hpp Set.hpp CacheL1.hpp)
