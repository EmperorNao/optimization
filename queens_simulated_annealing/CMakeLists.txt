cmake_minimum_required(VERSION 3.23)
project(optimization)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


# libs dirs
add_subdirectory("${PROJECT_SOURCE_DIR}/../config" libs/config)
add_subdirectory("${PROJECT_SOURCE_DIR}/../argparse" libs/argparse)


# executable
add_executable(queens_simulated_annealing main.cpp QueensAnnealingSolver.h utils.h)

# include lib directory
target_include_directories(queens_simulated_annealing PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../config"
        )

target_include_directories(queens_simulated_annealing PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../argparse/include/argparse"
        )


# library
target_link_libraries(queens_simulated_annealing PUBLIC config)
target_link_libraries(queens_simulated_annealing INTERFACE argparse)
