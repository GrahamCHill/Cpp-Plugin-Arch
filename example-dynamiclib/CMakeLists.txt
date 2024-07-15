cmake_minimum_required(VERSION 3.28)
project(example_dynamiclib)

set(CMAKE_CXX_STANDARD 17)

add_library(example_dynamiclib MODULE library.cpp)
