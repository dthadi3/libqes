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

#include <qes_file.h>
#include <qes_seqfile.h>
#include <time.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "kseq.h"

void bench_qes_file_readline_realloc_file(int silent);
void bench_qes_file_readline_file(int silent);
#ifdef GELINE_FOUND
void bench_gnu_getline_file(int silent);
#endif
void bench_qes_seqfile_parse_fq(int silent);
void bench_kseq_parse_fq(int silent);
void bench_qes_seqfile_write(int silent);
#ifdef OPENMP_FOUND
void bench_qes_seqfile_par_iter_fq_macro(int silent);
#endif


KSEQ_INIT(int, read)

static char *infile;

typedef struct __bench {
    const char *name;
    void (*fn)(int silent);
} bench_t;

void
bench_qes_file_readline_realloc_file(int silent)
{
    size_t bsz = 1<<4;
    char *buf = malloc(bsz);
    ssize_t len = 0;
    off_t flen = 0;
    struct qes_file *file = qes_file_open(infile, "r");

    assert(buf != NULL);
    while ((len = qes_file_readline_realloc(file, &buf, &bsz)) != EOF) {
        flen += len;
    }
    if (!silent)
        printf("[qes_file_readline_realloc]\tFile of %lu chars\n",
               (long unsigned)flen);
    qes_file_close(file);
    free(buf);
}

void
bench_qes_file_readline_file(int silent)
{
    size_t bsz = 1<<10;
    char buf[bsz];
    ssize_t len = 0;
    off_t flen = 0;

    struct qes_file *file = qes_file_open(infile, "r");
    while ((len = qes_file_readline(file, buf, bsz)) != EOF) {
        flen += len;
    }
    if (!silent)
        printf("[qes_file_readline]\t\tFile of %lu chars\n",
               (long unsigned)flen);
    qes_file_close(file);
}

#ifdef GELINE_FOUND
void
bench_gnu_getline_file(int silent)
{
    size_t bsz = 1<<4;
    char *buf = malloc(bsz);
    ssize_t len = 0;
    off_t flen = 0;
    FILE *file = fopen(infile, "r");

    assert(buf != NULL);
    while ((len = getline(&buf, &bsz, file)) != EOF) {
        flen += len;
    }
    if (!silent)
        printf("[getline]\t\tFile of %lu chars\n",
               (long unsigned)flen);
    fclose(file);
    free(buf);
}
#endif


#ifdef OPENMP_FOUND
void
bench_qes_seqfile_par_iter_fq_macro(int silent)
{
    struct qes_seqfile *sf = qes_seqfile_create(infile, "r");
    size_t total_len = 0;

    QES_SEQFILE_ITER_PARALLEL_SINGLE_BEGIN(sf, seq, seq_len, shared(total_len))
        #pragma omp critical
        {
        total_len += seq->seq.len;
        }
    QES_SEQFILE_ITER_PARALLEL_SINGLE_END(seq)

    if (!silent) {
        printf("[qes_seqfile_iter_fq_macro] Total seq len %lu\n",
               (long unsigned)total_len);
    }
    qes_seqfile_destroy(sf);
}
#endif

void
bench_qes_seqfile_parse_fq(int silent)
{
    struct qes_seq *seq = qes_seq_create();
    struct qes_seqfile *sf = qes_seqfile_create(infile, "r");
    ssize_t res = 0;
    size_t n_recs = 0;
    size_t seq_len = 0;

    while (res != EOF) {
        res = qes_seqfile_read(sf, seq);
        if (res < 1) {
            break;
        }
        seq_len += res;
        n_recs++;
    }
    if (!silent) {
        printf("[qes_seqfile_fq] Total seq len %lu\n",
               (long unsigned)seq_len);
    }
    qes_seqfile_destroy(sf);
    qes_seq_destroy(seq);
}

void
bench_kseq_parse_fq(int silent)
{
    int fp = open(infile, O_RDONLY);
    kseq_t *seq = kseq_init(fp);
    ssize_t res = 0;
    size_t n_recs = 0;
    size_t seq_len = 0;

    while ((res = kseq_read(seq)) >= 0) {
        seq_len += res;
        n_recs++;
    }
    if (!silent) {
        printf("[kseq_fq] Total seq len %lu\n",
               (long unsigned) seq_len);
    }
    kseq_destroy(seq);
    close(fp);
}

void
bench_qes_seqfile_write(int silent)
{
    struct qes_seq *seq = qes_seq_create();
    ssize_t res = 0;
    struct qes_seqfile *sf = NULL;
    char *fname = tmpnam(NULL);
    size_t iii = 0;

    /* Make a seq to write */
    qes_seq_fill_name(seq, "HWI-TEST", 8);
    qes_seq_fill_comment(seq, "testseq 1 2 3", 13);
    qes_seq_fill_seq(seq, "ACTCAATT", 8);
    qes_seq_fill_qual(seq, "IIIIIIII", 8);
    /* Setup file for writing */
    sf = qes_seqfile_create(fname, "wT");
    qes_seqfile_set_format(sf, FASTQ_FMT);
    for (iii = 0; iii < 1<<11; iii++) {
        res += qes_seqfile_write(sf, seq);
    }
    if (!silent) {
        printf("[qes_seqfile_write] Total file len %lu to %s\n",
               (long unsigned)res, fname);
    }
    qes_seqfile_destroy(sf);
    qes_seq_destroy(seq);
    remove(fname);

}

static const bench_t benchmarks[] = {
    { "qes_file_readline", &bench_qes_file_readline_file},
    { "qes_file_readline_realloc", &bench_qes_file_readline_realloc_file},
#ifdef GELINE_FOUND
    { "gnu_getline", &bench_gnu_getline_file},
#endif
    { "qes_seqfile_parse_fq", &bench_qes_seqfile_parse_fq},
#ifdef OPENMP_FOUND
    { "qes_seqfile_par_iter_fq_macro", &bench_qes_seqfile_par_iter_fq_macro},
#endif
    { "kseq_parse_fq", &bench_kseq_parse_fq},
    { "qes_seqfile_write", &bench_qes_seqfile_write},
    { NULL, NULL}
};

int
main (int argc, char *argv[])
{
    bench_t thisbench;
    clock_t start, end;
    size_t iii = 0;
    int rnds = 0;
    size_t nbench = 0;

    if (argc == 1) {
        fprintf(stderr, "USAGE:\nbench <file> <rounds> <bench> [<bench> ...]\n\n");
        fprintf(stderr, "\nAvalilable benchmarks are:\n");
        nbench = 0;
        while (1) {
            thisbench = benchmarks[nbench++];
            if ((thisbench.name && thisbench.fn)) {
                fprintf(stderr, "%s\n", thisbench.name);
            } else {
                return EXIT_FAILURE;
            }
        }
    }
    if (argc < 4) {
        fprintf(stderr, "USAGE:\nbench <file> <rounds> <bench> [<bench> ...]\n\n");
        return EXIT_FAILURE;
    } else {
        infile = strdup(argv[1]);
        rnds = atoi(argv[2]);
        printf("Begining benchmarks.\n");
        printf("---------------------------------------------------------------------\n");
    }
    for (iii = 3; iii < (unsigned int) argc; iii++) {
        nbench = 0;
        while (1) {
            thisbench = benchmarks[nbench++];
            if (!(thisbench.name && thisbench.fn)) {
                fprintf(stderr, "bad benchmark %s\n", argv[iii]);
                break;
            }
            if  (strcmp(argv[iii], thisbench.name) == 0) {
                int rnd = 0;
                start = clock();
                for (rnd = 0; rnd<rnds; rnd++) {
                    (*thisbench.fn)(rnd != rnds - 1);
                }
                end = clock();
                printf("Benchmark %s took %0.6fs per round [%d rounds]\n",
                        thisbench.name,
                        (float)(end - start) / (float)(CLOCKS_PER_SEC * rnds),
                        rnds);
                printf("---------------------------------------------------------------------\n");
                break;
            }
        }
    }
    free(infile);
    return EXIT_SUCCESS;
} /* ----------  end of function main  ---------- */
