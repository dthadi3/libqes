CC=gcc
DEBUG=
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

test: $(OBJ)
	$(CC) $(CFLAGS) $(BINFLAGS) -o test/run $(TEST_SRC) $(OBJ)
	$(CC) $(CFLAGS) $(BINFLAGS) -lkm -o test/run.dynamic $(TEST_SRC)
	./test/run

clean:
	rm -f obj/*

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(OBJFLAGS) -o $@ $<


