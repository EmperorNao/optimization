cmake_minimum_required(VERSION 3.23)
project(matrix)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_library(matrix matrix.cpp matrix.h)
set_target_properties(matrix PROPERTIES LINKER_LANGUAGE CXX)