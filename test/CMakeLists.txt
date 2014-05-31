#CFLAGS
#file(GLOB TEST_SRCS *.c)
set(TEST_SRCS test.c test_zfile.c test_seq.c test_seqfile.c test_util.c test_match.c helpers.c testdata.c)
set(TEST_SRCS ${TEST_SRCS} tinytest/tinytest.c)

add_executable(test_kmlib ${TEST_SRCS})
target_link_libraries(test_kmlib km ${KMLIB_DEPENDS_LIBS})
setup_target_for_coverage(coverage test_kmlib coverage)

# Benchmarking:
add_executable(bench_kmlib benchmarks.c)
target_link_libraries(bench_kmlib km ${KMLIB_DEPENDS_LIBS})

add_custom_command(TARGET test_kmlib COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)

add_test(NAME kmlib COMMAND ${CMAKE_BINARY_DIR}/bin/test_kmlib ${CMAKE_BINARY_DIR})
add_test(NAME kmlib_static  COMMAND ${CMAKE_BINARY_DIR}/bin/test_kmlib_static ${CMAKE_BINARY_DIR})
add_test(NAME kmlib_benchmarks COMMAND ${CMAKE_BINARY_DIR}/bin/bench_kmlib ${CMAKE_BINARY_DIR}/data/test.fastq)
