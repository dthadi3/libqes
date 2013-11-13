CC=gcc
CFLAGS=-g -O3 -I./include
LIBFLAGS=-shared
OBJFLAGS=-c

OBJ=$(patsubst %.c,./obj/%.o,$(SRCS))

all: obj
	$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o ./lib/$(PROG)

%.o: %.c
	$(CC) $(CFLAGS) $< -o %@
