cmake_minimum_required(VERSION 3.23)
project(nn)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_subdirectory("${PROJECT_SOURCE_DIR}/../nn" nn)


add_executable(nn_vision main.cpp)


target_include_directories(nn PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/../nn"
        )


target_link_libraries(nn_vision PUBLIC nn)

