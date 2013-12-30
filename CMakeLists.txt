cmake_minimum_required(VERSION 2.6)
project(kmlib)

# Cmake options
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${kmlib_BINARY_DIR}/lib)
enable_testing()

# CC options
add_definitions(-std=gnu99 -g -O3 -fgnu89-inline)
include_directories(${kmlib_SOURCE_DIR}/include)
link_directories(${kmlib_BINARY_DIR}/lib)

# Targets
add_library(km SHARED src/kmfile.c  src/kmmatch.c  src/kmseq.c)
add_subdirectory(test)

