cmake_minimum_required(VERSION 3.31.2)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_SCAN_FOR_MODULES ON) # Disabled due to incomplete support

include_directories("../../../mdspan/include/experimental")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

project(
  filter-less
  VERSION 0.1.0
  LANGUAGES CXX)

# add_library(StopWatch) target_sources(StopWatch PUBLIC FILE_SET CXX_MODULES
# FILES "src/modules/stopwatch.cxxm")

set(SOURCE_FILES "src/filter.cxx")
set(HEADER_FILES "src/include/bmp.hxx" "src/include/helpers.hxx")
# set(CMAKE_CXX_CLANG_TIDY clang-tidy)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# target_link_libraries(${PROJECT_NAME} StopWatch)