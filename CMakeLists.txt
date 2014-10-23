CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(libqes C)

# Append cmake-modules to module path
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules")

#############################
## Setup CMAKE Environment ##
#############################

INCLUDE(CheckSymbolExists)
# Shortcut to enable dev compile options
IF (DEV)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -fsanitize=address -fsanitize=leak -fsanitize=undefined")
	SET(CMAKE_BUILD_TYPE Coverage)
ENDIF()

IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# git versioning
INCLUDE(GitSemVer)
GetGitSemVer(LIBQES_VERSION)

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

CHECK_SYMBOL_EXISTS(getline stdio.h HAVE_GETLINE)
CHECK_SYMBOL_EXISTS(strndup string.h HAVE_STRNDUP)
FIND_PACKAGE(ZLIB 1.2.5 REQUIRED)
SET(LIBQES_DEPENDS_LIBS
    ${LIBQES_DEPENDS_LIBS} ${ZLIB_LIBRARIES})
SET(LIBQES_DEPENDS_INCLUDE_DIRS
    ${LIBQES_DEPENDS_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})

##########################
## Set Compiler Options ##
##########################

# Set GCC-specific options
IF (CMAKE_COMPILER_IS_GNUCC)
	SET(WEXTRA_FLAGS "${WEXTRA_FLAGS} -Woverride-init -Wnormalized=id -Wlogical-op")
	EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
			OUTPUT_VARIABLE GCC_VERSION)
	IF (${GCC_VERSION} VERSION_GREATER 4.9 OR ${GCC_VERSION} VERSION_EQUAL 4.9)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
	ENDIF()
	IF (NOT NO_OPENMP)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
	ENDIF()
ELSE()
	SET(NO_OPENMP 1)
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

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WEXTRA_FLAGS}")
SET(CMAKE_C_FLAGS_DEBUG "-ggdb -fstack-protector-all -march=native")
SET(CMAKE_C_FLAGS_RELEASE "-O4 -march=native")
# Coverage target set in CodeCoverage.cmake

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
