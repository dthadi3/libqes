## Compile tests, & set up coverage
SET(COMMON_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/helpers.c)
FILE(GLOB TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/test*.c)
SET(TEST_SRCS ${COMMON_SRCS} ${TEST_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/tinytest/tinytest.c)
ADD_EXECUTABLE(test_kmlib ${TEST_SRCS})
TARGET_LINK_LIBRARIES(test_kmlib km ${KMLIB_DEPENDS_LIBS})
IF (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	SETUP_TARGET_FOR_COVERAGE(coverage test_kmlib coverage src)
ENDIF()
ADD_TEST(NAME kmlib COMMAND ${CMAKE_BINARY_DIR}/bin/test_kmlib ${CMAKE_BINARY_DIR})

# Benchmarking:
ADD_EXECUTABLE(bench_kmlib ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks.c ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(bench_kmlib km ${KMLIB_DEPENDS_LIBS})
ADD_TEST(NAME kmlib_benchmarks
         COMMAND ${CMAKE_BINARY_DIR}/bin/bench_kmlib
	  ${CMAKE_BINARY_DIR}/data/test.fastq
	  50
	  seqfile_write
	  kseq_parse_fq
	  gnu_getline
	  seqfile_parse_fq
	  zfreadline_realloc)

# Copy test files over to bin dir
ADD_CUSTOM_COMMAND(TARGET test_kmlib
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
		    ${CMAKE_CURRENT_SOURCE_DIR}/data
		    ${CMAKE_BINARY_DIR}/data)
