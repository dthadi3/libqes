#file(GLOB KMLIB_SOURCES *.c)
SET(KMLIB_SOURCES kmzfile.c kmutil.c kmmatch.c kmsequtil.c kmseq.c kmseqfile.c kmstr.c kmdnatrie.c)

# Targets
IF(STATIC)
    ADD_LIBRARY(km ${KMLIB_SOURCES})
ELSE()
    ADD_LIBRARY(km SHARED ${KMLIB_SOURCES})
ENDIF()

FILE(GLOB KMLIB_HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)
INSTALL(FILES ${KMLIB_HEADERS} DESTINATION "include")
INSTALL(TARGETS km DESTINATION "lib")
