cmake_minimum_required(VERSION 3.23)
project(utils)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_subdirectory("${PROJECT_SOURCE_DIR}/../matrix" matrix)

add_library(utils utils.h utils.cpp dataloader.h dataloader.cpp)

target_include_directories(utils PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../matrix"
        )

target_link_libraries(utils PUBLIC matrix)

