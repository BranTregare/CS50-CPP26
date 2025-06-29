cmake_minimum_required(VERSION 3.31.6)
set(CMAKE_GENERATOR "Ninja Multi-Config")
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo MinSizeRel)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Set project name and required languages
project(
  recover
  VERSION 0.1.0
  LANGUAGES CXX
)

# Optional: Enable optimization flags if supported
include(CheckCXXCompilerFlag)

# Test for -march=native support
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
  message(STATUS "march=native supported. Enabling -march=native.")
  add_compile_options(-march=native)
else()
  message(WARNING "march=native not supported. Skipping -march=native optimization.")
endif()

# Test for -mavx2 support
check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
if(COMPILER_SUPPORTS_AVX2)
  message(STATUS "AVX2 supported. Enabling -mavx2.")
  add_compile_options(-mavx2)
else()
  message(WARNING "AVX2 not supported. Skipping -mavx2 optimization.")
endif()

# Source and header file configurations
set(SOURCE_FILES "src/recover.cxx")
set(HEADER_FILES "src/include/stopwatch.hxx")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})