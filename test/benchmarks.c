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

#include <kmzfile.h>
#include <kmseqfile.h>
#include <time.h>
#include <zlib.h>

#include "kseq.h"

KSEQ_INIT(gzFile, gzread)

static const char *infile;


typedef struct __bench {
    char *name;
    void (*fn)(int silent);
} bench_t;

void
zfreadline_realloc_file(int silent)
{
    size_t bsz = 1<<4;
    char *buf = malloc(bsz);
    ssize_t len = 0;
    off_t flen = 0;
    zfile_t *file = zfopen(infile, "r");
    while ((len = zfreadline_realloc(file, &buf, &bsz)) != EOF) {
        flen += len;
    }
    if (!silent)
        printf("[zfreadline_realloc]\tFile of %zu chars\n", flen);
    zfclose(file);
}

void
zfreadline_file(int silent)
{
    size_t bsz = 1<<10;
    char buf[bsz];
    ssize_t len = 0;
    off_t flen = 0;
    zfile_t *file = zfopen(infile, "r");
    while ((len = zfreadline(file, buf, bsz)) != EOF) {
        flen += len;
    }
    if (!silent)
        printf("[zfreadline]\t\tFile of %zu chars\n", flen);
    zfclose(file);
}

void
gnu_getline_file(int silent)
{
    size_t bsz = 1<<4;
    char *buf = malloc(bsz);
    ssize_t len = 0;
    off_t flen = 0;
    FILE *file = fopen(infile, "r");
    while ((len = getline(&buf, &bsz, file)) != EOF) {
        flen += len;
    }
    if (!silent)
        printf("[getline]\t\tFile of %zu chars\n", flen);
    fclose(file);
}

void
seqfile_parse_fq(int silent)
{
    seq_t *seq = create_seq();
    seqfile_t *sf = create_seqfile(infile, "r");
    ssize_t res = 0;
    size_t n_recs = 0;
    size_t seq_len = 0;
    while (res != EOF) {
        res = read_seqfile(sf, seq);
        if (res < 1) {
            break;
        }
        seq_len += res;
        n_recs++;
    }
    if (!silent) {
        printf("[seqfile_fq] Total seq len %zu\n", seq_len);
    }
    destroy_seqfile(sf);
    destroy_seq(seq);
}

void
kseq_parse_fq(int silent)
{
    gzFile fp = gzopen(infile, "r");
    kseq_t *seq = kseq_init(fp);
    ssize_t res = 0;
    size_t n_recs = 0;
    size_t seq_len = 0;
    while ((res = kseq_read(seq)) >= 0) {
        seq_len += res;
        n_recs++;
    }
    if (!silent) {
        printf("[kseq_fq] Total seq len %zu\n", seq_len);
    }
    kseq_destroy(seq);
    gzclose(fp);
}

static const bench_t benchmarks[] = {
    { "zfreadline", &zfreadline_file},
    { "zfreadline_realloc", &zfreadline_realloc_file},
    { "gnu_getline", &gnu_getline_file},
    { "seqfile_parse_fq", &seqfile_parse_fq},
    { "kseq_parse_fq", &kseq_parse_fq},
    { NULL, NULL}
};

int
main (int argc, char *argv[])
{
    bench_t thisbench;
    clock_t start, end;
    size_t nbench = 0;
    int n_rounds = 10;
    if (argc > 1) {
        infile = strdup(argv[1]);
    } else {
        fprintf(stderr, "USAGE:\nbench <file>\n\n");
        return EXIT_FAILURE;
    }
    if (argc > 2) {
        n_rounds = atoi(argv[2]);
    }
    while (1) {
        int iii;
        thisbench = benchmarks[nbench++];
        if (!(thisbench.name && thisbench.fn)) {
            break;;
        }
        start = clock();
        for (iii = 0; iii<n_rounds; iii++) {
            (*thisbench.fn)(iii != n_rounds - 1);
        }
        end = clock();
        printf("Test %s took %0.6fs per round [%d rounds]\n\n", thisbench.name,
                (float)(end - start) / (float)(CLOCKS_PER_SEC * n_rounds),
                n_rounds);
    }

    return EXIT_SUCCESS;
} /* ----------  end of function main  ---------- */
