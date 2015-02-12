CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(libqes C)

# Append cmake-modules to module path
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

# git describe as versioning
EXECUTE_PROCESS(COMMAND git describe
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE LIBQES_VERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE)

#############################
## Setup CMAKE Environment ##
#############################

OPTION(NO_OPENMP "Disable OpenMP" False)
OPTION(NO_ZLIB "Disable zlib" False)
# Shortcut to enable dev compile options
OPTION(DEV "Enable developer warnings")
IF (DEV)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -fsanitize=address -fsanitize=leak -fsanitize=undefined")
    SET(CMAKE_BUILD_TYPE Coverage)
ENDIF()

IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE(STATUS "${CMAKE_BUILD_TYPE} build of libqes version: ${LIBQES_VERSION}")

# Set output directories
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include coverage module IFF out build type is Coverage to avoid bailing out
# with unmet dependencies on Release builds, i.e. other peoples' computers
IF (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    INCLUDE(CodeCoverage)
ENDIF()

# Testing
ENABLE_TESTING()

# Packaging
SET(CPACK_GENERATOR "TGZ;TBZ2")
INCLUDE(CPack)

###############################
## Find Packages and Headers ##
###############################

# Header/symbols
INCLUDE(CheckSymbolExists)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckIncludeFiles)

CHECK_SYMBOL_EXISTS(getline stdio.h GETLINE_FOUND)
CHECK_SYMBOL_EXISTS(strndup string.h STRNDUP_FOUND)
CHECK_INCLUDE_FILES(malloc.h MALLOC_H_FOUND)
IF(MALLOC_H_FOUND)
    CHECK_SYMBOL_EXISTS(memalign malloc.h MEMALIGN_FOUND)
ELSE()
    SET(MEMALIGN_FOUND False)
ENDIF()
CHECK_SYMBOL_EXISTS(getpagesize unistd.h GETPAGESIZE_FOUND)

IF (NOT ${NO_ZLIB})
    FIND_PACKAGE(ZLIB 1.2.1 REQUIRED)
    CHECK_LIBRARY_EXISTS(${ZLIB_LIBRARIES} gzbuffer "" GZBUFFER_FOUND)
ELSE()
    SET(ZLIB_FOUND FALSE)
    SET(GZBUFFER_FOUND FALSE)
    SET(ZLIB_C_FLAGS "")
    SET(ZLIB_LIBRARIES "")
    SET(ZLIB_INCLUDE_DIRS "")
    MESSAGE(STATUS "Building without zlib")
ENDIF()

IF (NOT ${NO_OPENMP})
    FIND_PACKAGE(OpenMP)
ELSE()
    SET(OPENMP_FOUND FALSE)
    SET(OpenMP_C_FLAGS "")
    MESSAGE(STATUS "Building without OpenMP")
ENDIF()

# Set dependency flags appropriately
SET(LIBQES_DEPENDS_LIBS
    ${LIBQES_DEPENDS_LIBS}
    ${ZLIB_LIBRARIES})
SET(LIBQES_DEPENDS_INCLUDE_DIRS
    ${LIBQES_DEPENDS_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS})
SET(LIBQES_DEPENDS_CFLAGS
    ${LIBQES_DEPENDS_CFLAGS}
    ${ZLIB_CFLAGS}
    ${OpenMP_C_FLAGS})

##########################
## Set Compiler Options ##
##########################

# Set GCC-specific options
IF (CMAKE_COMPILER_IS_GNUCC)
    SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Woverride-init -Wnormalized=id -Wlogical-op")
    EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    IF (${GCC_VERSION} VERSION_GREATER 4.9 OR ${GCC_VERSION} VERSION_EQUAL 4.9)
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
    ENDIF()
ENDIF()

# Set CFLAGS
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wstack-protector -Wfloat-equal")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wundef -Wpointer-arith -Wstrict-prototypes")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wmissing-prototypes -Wwrite-strings -Wredundant-decls")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wchar-subscripts -Wcomment -Wformat=2 -Wwrite-strings")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wmissing-declarations -Wredundant-decls -Wnested-externs")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wbad-function-cast -Wswitch-enum -Winit-self")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wmissing-field-initializers -Wdeclaration-after-statement")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wold-style-definition -Waddress -Wmissing-noreturn ")
SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Wstrict-overflow=1 -Wextra -Warray-bounds -Wall")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LIBQES_DEPENDS_CFLAGS} -std=gnu99 ${WEXTRA_FLAGS}")

SET(CMAKE_C_FLAGS_DEBUG "-ggdb")
SET(CMAKE_C_FLAGS_RELEASE "-O3")

# Set include dirs
INCLUDE_DIRECTORIES(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/test/tinytest
    ${LIBQES_DEPENDS_INCLUDE_DIRS}
    )

# Set link dirs
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

# Traverse to library source and tests
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
