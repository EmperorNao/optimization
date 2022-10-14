cmake_minimum_required(VERSION 3.23)
project(genetic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_subdirectory("${PROJECT_SOURCE_DIR}/../config" config)
add_subdirectory("${PROJECT_SOURCE_DIR}/../argparse" argparse)
add_subdirectory("${PROJECT_SOURCE_DIR}/../utils" utils)


add_executable(genetic main.cpp GeneticSimulator.cpp GeneticSimulator.h)


# include lib directory
target_include_directories(genetic PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../config"
        )

target_include_directories(genetic INTERFACE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../argparse/include/argparse"
        )

target_include_directories(genetic PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../utils"
        )


# library
target_link_libraries(genetic PUBLIC config)
target_link_libraries(genetic PUBLIC utils)
target_link_libraries(genetic INTERFACE argparse)