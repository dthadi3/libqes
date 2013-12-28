#CFLAGS
add_executable(run main.c)
target_link_libraries(run km z)
#add_executable(run main.c)
#target_link_libraries(run z)
#target_link_libraries(run STATIC km)

add_test(NAME kmlib COMMAND run)
