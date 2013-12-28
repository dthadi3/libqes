CC=gcc
DEBUG=
#DEBUG=-DKMLIB_DEBUG
CFLAGS=-g -Wall -std=gnu99 -fgnu89-inline -O3 -I./src -I. -L./build/lib $(DEBUG)
LIBFLAGS=-shared
OBJFLAGS=-c -fPIC
BINFLAGS=-lz
SRCS=$(wildcard src/*.c)
TEST_SRC=test/main.c

SRC_NAMES=$(notdir $(SRCS))
OBJ=$(patsubst %.c,./build/obj/%.o, $(SRC_NAMES))
.PHONY: test clean

all: setup lib test $(OBJ)

lib: setup $(OBJ)
	$(CC) $(CFLAGS) $(LIBFLAGS) -o ./build/lib/libkm.so $(OBJ)

test/run: setup $(OBJ)
	$(CC) $(CFLAGS) $(BINFLAGS) -o test/run $(TEST_SRC) $(OBJ)

test/run.dynamic: setup $(OBJ) lib
	$(CC) $(CFLAGS) -Xlinker -rpath -Xlinker "./build/lib" $(BINFLAGS) -lkm -o test/run.dynamic $(TEST_SRC)

test: setup test/run test/run.dynamic
	./test/run ./test
	./test/run.dynamic ./test >/dev/null

clean:
	rm -vf build/* test/run test/run.dynamic

build/obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(OBJFLAGS) -o $@ $<

setup:
	mkdir -p ./build/lib ./build/obj ./build/bin
