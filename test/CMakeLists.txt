#CFLAGS
add_executable(test_kmlib main.c)
target_link_libraries(test_kmlib km z)
add_executable(test_kmlib_static main.c)
target_link_libraries(test_kmlib_static km_static z)

add_custom_command(TARGET test_kmlib COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)
add_test(NAME kmlib COMMAND test_kmlib)
add_test(NAME kmlib_static COMMAND test_kmlib_static)
