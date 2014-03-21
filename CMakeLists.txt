cmake_minimum_required(VERSION 2.6)
project(kmlib)

# Cmake options
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
enable_testing()

# CC options
set(CMAKE_C_FLAGS "-std=gnu99 -Wall")
set(CMAKE_C_FLAGS_DEBUG "-ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O3")
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)
