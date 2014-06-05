## Compile tests, & set up coverage
#file(GLOB TEST_SRCS *.c)
SET(TEST_SRCS test.c test_zfile.c test_seq.c test_seqfile.c test_util.c test_match.c helpers.c testdata.c test_sequtil.c)
SET(TEST_SRCS ${TEST_SRCS} tinytest/tinytest.c)
ADD_EXECUTABLE(test_kmlib ${TEST_SRCS})
TARGET_LINK_LIBRARIES(test_kmlib km ${KMLIB_DEPENDS_LIBS})
IF (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	SETUP_TARGET_FOR_COVERAGE(coverage test_kmlib coverage src)
ENDIF()
ADD_TEST(NAME kmlib COMMAND ${CMAKE_BINARY_DIR}/bin/test_kmlib ${CMAKE_BINARY_DIR})

# Benchmarking:
ADD_EXECUTABLE(bench_kmlib benchmarks.c helpers.c)
TARGET_LINK_LIBRARIES(bench_kmlib km ${KMLIB_DEPENDS_LIBS})
ADD_TEST(NAME kmlib_benchmarks COMMAND ${CMAKE_BINARY_DIR}/bin/bench_kmlib ${CMAKE_BINARY_DIR}/data/test.fastq)

# Copy test files over to bin dir
ADD_CUSTOM_COMMAND(TARGET test_kmlib COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)
