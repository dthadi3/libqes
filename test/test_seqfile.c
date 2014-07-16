/*
 * ============================================================================
 *
 *       Filename:  test_seqfile.c
 *
 *    Description:  Tests for the kmseqfile module
 *
 *        Version:  1.0
 *        Created:  16/05/14 12:33:47
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"

#include <kmseqfile.h>

#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

void
test_seqfile_create(void *ptr)
{
    seqfile_t *sf = NULL;
    char *fname = NULL;

    (void) ptr;
    /* Test file opening for reading */
    /* test opening a valid, unziped FASTQ */
    fname = find_data_file("test.fastq");
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_READ);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    seqfile_destroy(sf);
    free(fname);
    /* test opening a valid, unziped FASTA */
    fname = find_data_file("test.fasta");
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_READ);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(sf->flags.format, ==, FASTA_FMT);
    seqfile_destroy(sf);
    free(fname);
    /* Test opening a file in transparent write mode */
    fname = get_writable_file();
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "wT");
    KM_ZWRITE(sf->zf->fp, "ABCD", 4);
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(access(fname, F_OK), ==, 0);
    seqfile_destroy(sf);
    clean_writable_file(fname);
    fname = NULL;
    /* Test opening a file in write mode , acually zipping output */
    fname = get_writable_file();
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "w9");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(access(fname, F_OK), ==, 0);
    seqfile_destroy(sf);
    clean_writable_file(fname);
    fname = NULL;
end:
    seqfile_destroy(sf);
    if (fname != NULL) {
        free(fname);
    }
}

void
test_seqfile_guess_format (void *ptr)
{
    seqfile_t *sf = NULL;
    int res = -1;
    char *fname = NULL;

    (void) ptr;
    /* Test file opening for reading */
    fname = find_data_file("test.fastq");
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "r");
    /* test with a FASTQ file */
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    seqfile_destroy(sf);
    free(fname);
    /* test with a FASTA file */
    fname = find_data_file("test.fasta");
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTA_FMT);
    tt_int_op(sf->flags.format, ==, FASTA_FMT);
    seqfile_destroy(sf);
    free(fname);
    /* test with a gziped FASTQ file */
    fname = find_data_file("test.fastq.gz");
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    seqfile_destroy(sf);
end:
    seqfile_destroy(sf);
    if (fname != NULL) free(fname);
}

void
test_seqfile_destroy (void *ptr)
{
    seqfile_t *sf = NULL;
    char *fname = NULL;

    (void) ptr;
    /* Test file opening for reading */
    fname = find_data_file("test.fastq");
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_assert(seqfile_ok(sf));
    seqfile_destroy(sf);
    tt_ptr_op(sf, ==, NULL);
    tt_assert(!seqfile_ok(sf));
end:
    seqfile_destroy(sf);
    if (fname != NULL) free(fname);
}



/*===  FUNCTION  ============================================================*
Name:           test_seqfile_read
Description:    Tests the seqfile_read function from kmseqfile.c
 *===========================================================================*/
void
test_seqfile_read (void *ptr)
{
    seq_t *seq = seq_create();
    ssize_t res = 0;
    seqfile_t *sf = NULL;
    char *fname = NULL;
    /* Check a seq is empty */
#define CHECK_SEQ_EMPTY                                                     \
    tt_str_op(seq->name.s, ==, "");                                         \
    tt_str_op(seq->comment.s, ==, "");                                      \
    tt_str_op(seq->seq.s, ==, "");                                          \
    tt_str_op(seq->qual.s, ==, "")
    /* Check seq against the first known read */
#define CHECK_SEQ_FIRST                                                     \
    tt_str_op(seq->name.s, ==, first_fastq_read[0]);                        \
    tt_str_op(seq->comment.s, ==, first_fastq_read[1]);                     \
    tt_str_op(seq->seq.s, ==, first_fastq_read[2]);                         \
    tt_str_op(seq->qual.s, ==, first_fastq_read[3])
    /* Open, read, check & close a seqfile */
#define CHECK_SEQFILE_READ(fn, expt_res, check_seq)                         \
    fname = find_data_file(fn);                                             \
    tt_assert(fname != NULL);                                               \
    sf = seqfile_create(fname, "r");                                        \
    res = seqfile_read(sf, seq);                                            \
    tt_int_op(res, ==, expt_res);                                           \
    check_seq;                                                              \
    seqfile_destroy(sf);                                                    \
    free(fname);                                                            \
    fname = NULL
    /* Open, read, check & close a seqfile, forcing its filetype to FASTQ */
#define CHECK_SEQFILE_READ_FORCE(fn, expt_res, check_seq)                   \
    fname = find_data_file(fn);                                             \
    tt_assert(fname != NULL);                                               \
    sf = seqfile_create(fname, "r");                                        \
    seqfile_set_format(sf, FASTQ_FMT);                                      \
    res = seqfile_read(sf, seq);                                            \
    tt_int_op(res, ==, expt_res);                                           \
    check_seq;                                                              \
    seqfile_destroy(sf);                                                    \
    free(fname);                                                            \
    fname = NULL

    (void) ptr;
    /* Test file opening for reading */
    CHECK_SEQFILE_READ("test.fastq", first_fastq_len, CHECK_SEQ_FIRST);
    /* Test with bad fastqs, ensure all code paths are taken */
    CHECK_SEQFILE_READ("loremipsum.txt", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ("bad_nohdr.fastq", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ("loremipsum.txt", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ_FORCE("loremipsum.txt", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ("empty.fastq", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ("bad_noqual.fastq", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ("bad_noqualhdrchr.fastq", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ("bad_noqualhdreol.fastq", -2, CHECK_SEQ_EMPTY);
    CHECK_SEQFILE_READ("bad_diff_lens.fastq", -2, CHECK_SEQ_EMPTY);
    /* Check with bad params that it returns -2 */
    res = seqfile_read(NULL, seq);
    tt_int_op(res, ==, -2);
    res = seqfile_read(sf, NULL);
    tt_int_op(res, ==, -2);
end:
    seqfile_destroy(sf);
    seq_destroy(seq);
    if (fname != NULL) {
        free(fname);
    }
#undef CHECK_SEQ_EMPTY
#undef CHECK_SEQFILE_READ
}

void
test_seqfile_read_vs_kseq (void *ptr)
{
    seq_t *seq = seq_create();
    char *fname = find_data_file("test.fastq.gz");
    seqfile_t *sf = seqfile_create(fname, "r");
    gzFile fp = gzopen(fname, "r");
    kseq_t *kseq = kseq_init(fp);
    ssize_t kseq_res = 0;
    ssize_t my_res = 0;

    (void) ptr;
    tt_assert(fname != NULL);
    while (1) {
        my_res = seqfile_read(sf, seq);
        kseq_res = kseq_read(kseq);
        tt_int_op(my_res, ==, kseq_res);
        if (my_res < 1 || kseq_res < 1) {
            /* EOF or error */
            break;
        }
        tt_str_op(seq->name.s, ==, kseq->name.s);
        tt_str_op(seq->comment.s, ==, kseq->comment.s);
        tt_str_op(seq->seq.s, ==, kseq->seq.s);
        tt_str_op(seq->qual.s, ==, kseq->qual.s);
    }
    my_res = seqfile_read(sf, seq);
    kseq_res = kseq_read(kseq);
    tt_int_op(my_res, ==, kseq_res);
    tt_int_op(my_res, ==, EOF);
    seqfile_destroy(sf);
    seq_destroy(seq);
    kseq_destroy(kseq);
    gzclose(fp);
    free(fname);
    /* Try again, with fasta */
    seq = seq_create();
    fname = find_data_file("test.fasta");
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "r");
    fp = gzopen(fname, "r");
    kseq = kseq_init(fp);
    kseq_res = 0;
    my_res = 0;
    while (1) {
        my_res = seqfile_read(sf, seq);
        kseq_res = kseq_read(kseq);
        tt_int_op(my_res, ==, kseq_res);
        if (my_res < 1 || kseq_res < 1) {
            /* EOF or error */
            break;
        }
        tt_str_op(seq->name.s, ==, kseq->name.s);
        tt_str_op(seq->comment.s, ==, kseq->comment.s);
        tt_str_op(seq->seq.s, ==, kseq->seq.s);
    }
end:
    seqfile_destroy(sf);
    seq_destroy(seq);
    kseq_destroy(kseq);
    gzclose(fp);
    if (fname != NULL) {
        free(fname);
    }
}


/*===  FUNCTION  ============================================================*
Name:           test_seqfile_write
Description:    Tests the seqfile_write function from kmseqfile.c
 *===========================================================================*/

void
test_seqfile_write (void *ptr)
{
    seq_t *seq = seq_create();
    size_t expt_bytes = 0;
    ssize_t res = 0;
    seqfile_t *sf = NULL;
    char *fname = NULL;
    char *crc = NULL;

    (void) ptr;
    /* Make a seq to write */
    seq_fill_name(seq, "HWI-TEST", 8);
    seq_fill_comment(seq, "testseq 1 2 3", 13);
    seq_fill_seq(seq, "ACTCAATT", 8);
    seq_fill_qual(seq, "IIIIIIII", 8);
    expt_bytes = 1 + 8 + 1 + 13 + 1 +   /* @ + name + ' ' + comment + '\n' */
                 8 + 1 + 2 + 8 + 1;     /* seq + '\n' + "+\n" + qual + '\n' */
    /* Test with a FASTQ seqfile */
    fname = get_writable_file();
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "wT");
    seqfile_set_format(sf, FASTQ_FMT);
    res = seqfile_write(sf, seq);
    tt_int_op(res, ==, expt_bytes);
    seqfile_destroy(sf); /* Has to happen here to flush it */
    crc = crc32_file(fname);
    tt_str_op(crc, ==, "d4665941");
    clean_writable_file(fname);
    free(crc);
    fname = NULL;
    crc = NULL;
    /* Test with a FASTA seqfile */
    expt_bytes = 1 + 8 + 1 + 13 + 1 +   /* @ + name + ' ' + comment + '\n' */
                 8 + 1;                 /* seq + '\n'*/
    fname = get_writable_file();
    tt_assert(fname != NULL);
    sf = seqfile_create(fname, "wT");
    seqfile_set_format(sf, FASTA_FMT);
    /* do the write */
    res = seqfile_write(sf, seq);
    tt_int_op(res, ==, expt_bytes);
    seqfile_destroy(sf); /* Flush it */
    crc = crc32_file(fname);
    tt_str_op(crc, ==, "0a295c77");
    clean_writable_file(fname);
    fname = NULL;
    /* Check with bad params that it returns -2 */
    res = seqfile_write(NULL, seq);
    tt_int_op(res, ==, -2);
    res = seqfile_write(sf, NULL);
    tt_int_op(res, ==, -2);
end:
    seqfile_destroy(sf);
    seq_destroy(seq);
    if (fname != NULL) free(fname);
    if (crc != NULL) free(crc);
}

struct testcase_t seqfile_tests[] = {
    { "seqfile_create", test_seqfile_create, 0, NULL, NULL},
    { "seqfile_guess_format", test_seqfile_guess_format, 0, NULL, NULL},
    { "seqfile_destroy", test_seqfile_destroy, 0, NULL, NULL},
    { "seqfile_read_vs_kseq", test_seqfile_read_vs_kseq, 0, NULL, NULL},
    { "seqfile_read", test_seqfile_read, 0, NULL, NULL},
    { "seqfile_write", test_seqfile_write, 0, NULL, NULL},
    END_OF_TESTCASES
};
