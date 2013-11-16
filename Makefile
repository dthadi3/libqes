CC=gcc
DEBUG=
#DEBUG=-DKMLIB_DEBUG
CFLAGS=-g -Wall -Wpedantic -std=gnu99 -fgnu89-inline -O3 -I./include -L./lib $(DEBUG)
LIBFLAGS=-shared
OBJFLAGS=-c -fPIC
BINFLAGS=-lz
SRCS=$(wildcard src/*.c)
TEST_SRC=test/main.c

SRC_NAMES=$(notdir $(SRCS))
OBJ=$(patsubst %.c,./obj/%.o, $(SRC_NAMES))
.PHONY: all test clean

all: lib test $(OBJ)

lib: $(OBJ)
	$(CC) $(CFLAGS) $(LIBFLAGS) -o ./lib/libkm.so $(OBJ)

test/run: $(OBJ)
	$(CC) $(CFLAGS) $(BINFLAGS) -o test/run $(TEST_SRC) $(OBJ)

test/run.dynamic: $(OBJ)
	$(CC) $(CFLAGS) $(BINFLAGS) -lkm -o test/run.dynamic $(TEST_SRC)

test: test/run test/run.dynamic
	./test/run

clean:
	rm -vf obj/* test/run test/run.dynamic

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(OBJFLAGS) -o $@ $<
