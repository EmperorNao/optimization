cmake_minimum_required(VERSION 3.23)
project(config)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_library(config config.h)
set_target_properties(config PROPERTIES LINKER_LANGUAGE CXX)