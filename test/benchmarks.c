/*
 * ============================================================================
 *
 *       Filename:  benchmarks.c
 *
 *    Description:  Some benchmarks
 *
 *        Version:  1.0
 *        Created:  16/05/14 12:27:04
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#include <stdlib.h>

#include "kseq.h"

#include <kmzfile.h>
#include <time.h>
static const char *infile;


typedef struct __bench {
    char *name;
    void (*fn)();
} bench_t;

void
zfreadline_realloc_file()
{
    size_t bsz = 1<<4;
    char *buf = malloc(bsz);
    ssize_t len = 0;
    off_t flen = 0;
    zfile_t *file = zfopen(infile, "r");
    while ((len = zfreadline_realloc(file, &buf, &bsz)) != EOF) {
        flen += len;
    }
    printf("[zfreadline_realloc]\tFile of %zu chars\n", flen);
    zfclose(file);
}

void
zfreadline_file()
{
    size_t bsz = 1<<10;
    char buf[bsz];
    ssize_t len = 0;
    off_t flen = 0;
    zfile_t *file = zfopen(infile, "r");
    while ((len = zfreadline(file, buf, bsz)) != EOF) {
        flen += len;
    }
    printf("[zfreadline]\t\tFile of %zu chars\n", flen);
    zfclose(file);
}

void
gnu_getline_file()
{
    size_t bsz = 1<<4;
    char *buf = malloc(bsz);
    ssize_t len = 0;
    off_t flen = 0;
    FILE *file = fopen(infile, "r");
    while ((len = getline(&buf, &bsz, file)) != EOF) {
        flen += len;
    }
    printf("[getline]\t\tFile of %zu chars\n", flen);
    fclose(file);
}
static const bench_t benchmarks[] = {
    { "zfreadline", &zfreadline_file},
    { "zfreadline_realloc", &zfreadline_realloc_file},
    { "gnu_getline", &gnu_getline_file},
    { NULL, NULL}
};

int
main (int argc, char *argv[])
{
    if (argc > 1) {
        infile = strdup(argv[1]);
    } else {
        fprintf(stderr, "USAGE:\nbench <file>\n\n");
        return EXIT_FAILURE;
    }
    bench_t thisbench;
    clock_t start, end;
    size_t nbench = 0;
    while (1) {
        thisbench = benchmarks[nbench++];
        if (!(thisbench.name && thisbench.fn)) {
            break;;
        }
        start = clock();
        (*thisbench.fn)();
        end = clock();
        printf("Test %s took %0.3fs\n\n", thisbench.name,
                (float)(end - start) / (float)CLOCKS_PER_SEC);
    }

    return EXIT_SUCCESS;
} /* ----------  end of function main  ---------- */
