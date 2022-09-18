cmake_minimum_required(VERSION 3.23)
project(ants)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_subdirectory("${PROJECT_SOURCE_DIR}/../config" config)
add_subdirectory("${PROJECT_SOURCE_DIR}/../argparse" argparse)
add_subdirectory("${PROJECT_SOURCE_DIR}/../utils" utils)



add_executable(ants main.cpp antssimulator.cpp antssimulator.h)

# include lib directory
target_include_directories(ants PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../config"
        )

target_include_directories(ants INTERFACE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../argparse/include/argparse"
        )

target_include_directories(ants PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../utils"
        )


# library
target_link_libraries(ants PUBLIC config)
target_link_libraries(ants PUBLIC matrix)
target_link_libraries(ants PUBLIC utils)
target_link_libraries(ants INTERFACE argparse)

