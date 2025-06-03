cmake_minimum_required(VERSION 3.31.6)

project(credit VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# Paths
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${SOURCE_DIR}/include")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")
set(TEST_INCLUDE_DIR "${TEST_DIR}/include")

# Main executable
add_executable(credit ${SOURCE_DIR}/credit.cxx)
target_compile_options(credit PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(credit PRIVATE ${INCLUDE_DIR})

# Header-only library: stopwatch
add_library(stopwatch INTERFACE)
target_include_directories(stopwatch INTERFACE ${INCLUDE_DIR})
target_link_libraries(credit PRIVATE stopwatch)

# === Static Assert Test Target ===
add_executable(credit_test ${TEST_DIR}/credit_test.cxx)
target_compile_options(credit_test PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(credit_test PRIVATE
  ${INCLUDE_DIR}
  ${TEST_INCLUDE_DIR}
)
target_link_libraries(credit_test PRIVATE stopwatch)

# === Catch2 Runtime Unit Tests ===
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.4
)
FetchContent_MakeAvailable(Catch2)

add_executable(credit_runtime_test ${TEST_DIR}/credit_runtime_test.cxx)
target_link_libraries(credit_runtime_test PRIVATE Catch2::Catch2WithMain stopwatch)
target_compile_options(credit_runtime_test PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(credit_runtime_test PRIVATE
  ${INCLUDE_DIR}
  ${TEST_INCLUDE_DIR}
)
include(Catch)
catch_discover_tests(credit_runtime_test)