cmake_minimum_required(VERSION 3.31.6)

project(cash VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# Paths
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${SOURCE_DIR}/include")

# Main executable
add_executable(cash ${SOURCE_DIR}/cash.cxx)
target_compile_options(cash PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(cash PRIVATE ${INCLUDE_DIR})

# Header-only library: stopwatch
add_library(stopwatch INTERFACE)
target_include_directories(stopwatch INTERFACE ${INCLUDE_DIR})
target_link_libraries(cash PRIVATE stopwatch)
