CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(kmlib C)
SET(KMLIB_MAJOR_VERSION 0)
SET(KMLIB_MINOR_VERSION 1)
SET(KMLIB_PATCH_VERSION 1)
SET(KMLIB_VERSION "${KMLIB_MAJOR_VERSION}.${KMLIB_MINOR_VERSION}.${KMLIB_PATCH_VERSION}a3")

#############################
## Setup CMAKE Environment ##
#############################

# Shortcut to enable dev compile options
IF (DEV)
	SET(CMAKE_BUILD_TYPE Coverage)
ENDIF()

IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

# Append cmake-modules to module path
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")

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

FIND_PACKAGE(ZLIB 1.2.5 REQUIRED)
SET(KMLIB_DEPENDS_LIBS
    ${KMLIB_DEPENDS_LIBS} ${ZLIB_LIBRARIES})
SET(KMLIB_DEPENDS_INCLUDE_DIRS
    ${KMLIB_DEPENDS_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
SET(KMLIB_DEPENDS_CFLAGS
    "${KMLIB_DEPENDS_CFLAGS} -DHAVE_ZLIB")

##########################
## Set Compiler Options ##
##########################

# When STATIC is TRUE, then cmake only looks for static libraries
IF(LINUX)
    IF(STATIC)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    ENDIF(STATIC)
ENDIF(LINUX)

# Set CFLAGS
SET(EXTRA_WARNING_FLAGS "-fstack-protector-all -Wstack-protector -Wfloat-equal -Wundef -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wredundant-decls -Wchar-subscripts -Wcomment -Wformat=2 -Wwrite-strings -Wmissing-declarations -Wredundant-decls -Wnested-externs -Wbad-function-cast -Wswitch-enum -Winit-self -Wmissing-field-initializers -Wdeclaration-after-statement -Wold-style-definition -Waddress -Wmissing-noreturn -Wstrict-overflow=1 -Wnormalized=id -Woverride-init -Wextra -Warray-bounds -Wlogical-op")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall ${KMLIB_DEPENDS_CFLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DKMLIB_VERSION=\\\"${KMLIB_VERSION}\\\" ${EXTRA_WARNING_FLAGS}")
IF (NOT NO_OPENMP)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
ELSE()
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DKMLIB_NO_OPENMP")
ENDIF()
SET(CMAKE_C_FLAGS_DEBUG "-ggdb")
SET(CMAKE_C_FLAGS_RELEASE "-O3")
# Coverage target set in CodeCoverage.cmake

# Set include dirs
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/test/tinytest
    ${KMLIB_DEPENDS_INCLUDE_DIRS}
    )

# Set link dirs
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

# Traverse to library source and tests
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
