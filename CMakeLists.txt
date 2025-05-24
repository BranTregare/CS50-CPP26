cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

project(
  credit
  VERSION 0.1.0
  LANGUAGES CXX)

# add_library(stopwatch) target_sources(stopwatch PUBLIC FILE_SET CXX_MODULES
# FILES "src/modules/stopwatch.cxx")

set(SOURCE_FILES "src/credit.cxx")
set(HEADER_FILES "src/include/stopwatch.hxx")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# target_link_libraries(${PROJECT_NAME} stopwatch)
