## Compile tests, & set up coverage
#file(GLOB TEST_SRCS *.c)
set(TEST_SRCS test.c test_zfile.c test_seq.c test_seqfile.c test_util.c test_match.c helpers.c testdata.c test_sequtil.c)
set(TEST_SRCS ${TEST_SRCS} tinytest/tinytest.c)
add_executable(test_kmlib ${TEST_SRCS})
target_link_libraries(test_kmlib km ${KMLIB_DEPENDS_LIBS})
IF (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	setup_target_for_coverage(coverage test_kmlib coverage)
ENDIF()
add_test(NAME kmlib COMMAND ${CMAKE_BINARY_DIR}/bin/test_kmlib ${CMAKE_BINARY_DIR})

# Benchmarking:
add_executable(bench_kmlib benchmarks.c)
target_link_libraries(bench_kmlib km ${KMLIB_DEPENDS_LIBS})
add_test(NAME kmlib_benchmarks COMMAND ${CMAKE_BINARY_DIR}/bin/bench_kmlib ${CMAKE_BINARY_DIR}/data/test.fastq)

# Copy test files over to bin dir
add_custom_command(TARGET test_kmlib COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)
