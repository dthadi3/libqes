#AM_CFLAGS = ${kmlib_CFLAGS}

#AM_CFLAGS = -std=gnu99 -Wall -Wpedantic -Werror
AM_CFLAGS = @KMLIB_GLOBAL_CFLAGS@
GZIP_FLAGS = -lz

TESTS = rundynamic run

bin_PROGRAMS = \
	rundynamic \
	run

run_SOURCES = \
	main.c


rundynamic_SOURCES = \
	main.c

run_LDADD = \
	../src/libkm.la \
	$(GZIP_FLAGS)

rundynamic_LDADD = \
	../src/libkm.la \
	$(GZIP_FLAGS)
