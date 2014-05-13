#CFLAGS
set(TEST_SRCS test.c test_util.c test_match.c tinytest/tinytest.c)
add_executable(test_kmlib ${TEST_SRCS})
target_link_libraries(test_kmlib km z)
add_executable(test_kmlib_static ${TEST_SRCS})
target_link_libraries(test_kmlib_static km_static z)

add_custom_command(TARGET test_kmlib COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)
add_test(NAME kmlib COMMAND ${CMAKE_BINARY_DIR}/bin/test_kmlib)
add_test(NAME kmlib_static COMMAND ${CMAKE_BINARY_DIR}/bin/test_kmlib_static)
