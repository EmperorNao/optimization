cmake_minimum_required(VERSION 3.23)
project(nn)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_subdirectory("${PROJECT_SOURCE_DIR}/../utils" utils)


add_library(nn core/functional.h functional.h numerical/numerical.h numerical/numerical.cpp functional.cpp nn.cpp nn.h layer_options.cpp layer_options.h)



target_include_directories(nn PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../utils"
        )


target_link_libraries(nn PUBLIC utils)
