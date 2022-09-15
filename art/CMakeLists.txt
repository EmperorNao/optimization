cmake_minimum_required(VERSION 3.23)
project(art)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


# libs dirs
add_subdirectory("${PROJECT_SOURCE_DIR}/../config" config)
add_subdirectory("${PROJECT_SOURCE_DIR}/../argparse" argparse)
add_subdirectory("${PROJECT_SOURCE_DIR}/../matrix" matrix)
add_subdirectory("${PROJECT_SOURCE_DIR}/../utils" utils)


add_executable(art main.cpp dataloader.cpp dataloader.h artclustering.cpp artclustering.h)


# include lib directory
target_include_directories(art PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../config"
        )

target_include_directories(art INTERFACE
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../argparse/include/argparse"
        )

target_include_directories(art PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../matrix"
        )

target_include_directories(art PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../utils"
        )

# library
target_link_libraries(art PUBLIC config)
target_link_libraries(art PUBLIC matrix)
target_link_libraries(art PUBLIC utils)
target_link_libraries(art INTERFACE argparse)
