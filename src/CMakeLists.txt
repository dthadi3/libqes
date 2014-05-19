#file(GLOB KMLIB_SOURCES *.c)
set(KMLIB_SOURCES kmzfile.c kmutil.c kmmatch.c kmsequtil.c kmseq.c kmseqfile.c kmstr.c)

# Targets
add_library(km SHARED ${KMLIB_SOURCES})
add_library(km_static ${KMLIB_SOURCES})

INSTALL(TARGETS km DESTINATION "lib")
INSTALL(TARGETS km_static DESTINATION "lib")
