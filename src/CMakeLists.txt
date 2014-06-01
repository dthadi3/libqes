#file(GLOB KMLIB_SOURCES *.c)
set(KMLIB_SOURCES kmzfile.c kmutil.c kmmatch.c kmsequtil.c kmseq.c kmseqfile.c kmstr.c)

# Targets
if(STATIC)
add_library(km ${KMLIB_SOURCES})
else(STATIC)
add_library(km SHARED ${KMLIB_SOURCES})
endif(STATIC)

file(GLOB KMLIB_HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
INSTALL(FILES ${KMLIB_HEADERS} DESTINATION "include")
INSTALL(TARGETS km DESTINATION "lib")
