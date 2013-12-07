#SUBDIRS = bin

bin_PROGRAMS = \
	run \
	run.dynamic

run_CFLAGS = \
	-I ../include

run_LDADD = \
	../lib/libkm.a

run_SOURCES = \
	main.c


run_dynamic_SOURCES = \
	main.c

run_dynamic_CFLAGS = \
	-lkm \
	-L ../lib

