cmake_minimum_required(VERSION 2.8)
project(kmlib C)
set(KMLIB_MAJOR_VERSION 0)
set(KMLIB_MINOR_VERSION 1)
set(KMLIB_PATCH_VERSION 0)
set(KMLIB_VERSION ${KMLIB_MAJOR_VERSION}.${KMLIB_MINOR_VERSION}.${KMLIB_PATCH_VERSION})

# Cmake options
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include(CodeCoverage)
enable_testing()

# Packaging
INCLUDE(CPack)
SET(CPACK_GENERATOR "TGZ;TBZ2")

# find libs
find_package(ZLIB 1.2.5 REQUIRED)
set(KMLIB_DEPENDS_LIBS ${KMLIB_DEPENDS_LIBS} ${ZLIB_LIBRARIES})
set(KMLIB_DEPENDS_INCLUDE_DIRS ${KMLIB_DEPENDS_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
set(KMLIB_DEPENDS_CFLAGS "${KMLIB_DEPENDS_CFLAGS} -DHAVE_ZLIB")

# CC options
SET(STATIC FALSE CACHE BOOL "Build a static library?")

# When STATIC is TRUE, then cmake only looks for static libraries
IF(LINUX)
    IF(STATIC)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    ENDIF(STATIC)
ENDIF(LINUX)

# set -static, when STATIC is TRUE and set LINK_SEARCH_END_STATIC
# to remove the additional -bdynamic from the linker line.
set(CMAKE_C_FLAGS "-std=gnu99 -Wall ${KMLIB_DEPENDS_CFLAGS} -DKMLIB_VERSION=${KMLIB_VERSION}")
set(CMAKE_C_FLAGS_DEBUG "-ggdb -Werror -fprofile-arcs -ftest-coverage -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/test ${CMAKE_SOURCE_DIR}/test/tinytest ${KMLIB_DEPENDS_INCLUDE_DIRS})
link_directories(${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)
