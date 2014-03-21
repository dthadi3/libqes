
file(GLOB KMLIB_SOURCES *.c)
# Targets
add_library(km SHARED ${KMLIB_SOURCES})

INSTALL(TARGETS km DESTINATION "lib")
INSTALL(FILES
		include/kmbase.h
	      	include/kmfile.h
	      	include/kmlib.h
		include/kmmatch.h
		include/kmseq.h
		include/kmtest.h
		include/kmutil.h
	DESTINATION "include")
