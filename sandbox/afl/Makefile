AFL_ZLIB=
CC=/usr/local/bin/afl-gcc
SRC=../../src
SRCS = $(SRC)/qes_compat.c $(SRC)/qes_file.c $(SRC)/qes_match.c $(SRC)/qes_seq.c $(SRC)/qes_seqfile.c $(SRC)/qes_sequtil.c $(SRC)/qes_str.c $(SRC)/qes_util.c ./benchmarks.c
CFLAGS=-g -O3 -I$(SRC) -I. -I./src

run: $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -f ./run
