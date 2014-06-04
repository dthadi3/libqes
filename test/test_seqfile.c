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
test_create_seqfile(void *ptr)
{
    seqfile_t *sf = NULL;
    char *fname = NULL;

    (void) ptr;
    /* Test file opening for reading */
    /* test opening a valid, unziped FASTQ */
    fname = find_data_file("test.fastq");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_READ);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    destroy_seqfile(sf);
    free(fname);
    /* test opening a valid, unziped FASTA */
    fname = find_data_file("test.fasta");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_READ);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(sf->flags.format, ==, FASTA_FMT);
    destroy_seqfile(sf);
    free(fname);
    /* Test opening a file in transparent write mode */
    fname = get_writable_file();
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "wT");
    KM_ZWRITE(sf->zf->fp, "ABCD", 4);
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(access(fname, F_OK), ==, 0);
    destroy_seqfile(sf);
    clean_writable_file(fname);
    fname = NULL;
    /* Test opening a file in write mode , acually zipping output */
    fname = get_writable_file();
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "w9");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(access(fname, F_OK), ==, 0);
    destroy_seqfile(sf);
    clean_writable_file(fname);
    fname = NULL;
end:
    destroy_seqfile(sf);
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
    sf = create_seqfile(fname, "r");
    /* test with a FASTQ file */
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    destroy_seqfile(sf);
    free(fname);
    /* test with a FASTA file */
    fname = find_data_file("test.fasta");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTA_FMT);
    tt_int_op(sf->flags.format, ==, FASTA_FMT);
    destroy_seqfile(sf);
    free(fname);
    /* test with a gziped FASTQ file */
    fname = find_data_file("test.fastq.gz");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    destroy_seqfile(sf);
end:
    destroy_seqfile(sf);
    if (fname != NULL) free(fname);
}

void
test_destroy_seqfile (void *ptr)
{
    seqfile_t *sf = NULL;
    char *fname = NULL;

    (void) ptr;
    /* Test file opening for reading */
    fname = find_data_file("test.fastq");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_assert(seqfile_ok(sf));
    destroy_seqfile(sf);
    tt_ptr_op(sf, ==, NULL);
    tt_assert(!seqfile_ok(sf));
end:
    destroy_seqfile(sf);
    if (fname != NULL) free(fname);
}



/*===  FUNCTION  ============================================================*
Name:           test_read_seqfile
Description:    Tests the read_seqfile function from kmseqfile.c
 *===========================================================================*/
void
test_read_seqfile (void *ptr)
{
    seq_t *seq = create_seq();
    ssize_t res = 0;
    seqfile_t *sf = NULL;
    char *fname = NULL;

    (void) ptr;
    /* Test file opening for reading */
    fname = find_data_file("test.fastq");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    /* Test with a good seqfile */
    res = read_seqfile(sf, seq);
    tt_int_op(res, ==, first_fastq_len);
    tt_str_op(seq->name.s, ==, first_fastq_read[0]);
    tt_str_op(seq->comment.s, ==, first_fastq_read[1]);
    tt_str_op(seq->seq.s, ==, first_fastq_read[2]);
    tt_str_op(seq->qual.s, ==, first_fastq_read[3]);
    destroy_seqfile(sf);
    free(fname);
    /* Test with bad seqfiles */
    fname = find_data_file("loremipsum.txt");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    res = read_seqfile(sf, seq);
    tt_int_op(res, ==, -2);
    tt_str_op(seq->name.s, ==, "");
    tt_str_op(seq->comment.s, ==, "");
    tt_str_op(seq->seq.s, ==, "");
    tt_str_op(seq->qual.s, ==, "");
    /* Check with bad params that it returns -2 */
    res = read_seqfile(NULL, seq);
    tt_int_op(res, ==, -2);
    res = read_seqfile(sf, NULL);
    tt_int_op(res, ==, -2);
end:
    destroy_seqfile(sf);
    destroy_seq(seq);
    if (fname != NULL) {
        free(fname);
    }
}

void
test_read_seqfile_vs_kseq (void *ptr)
{
    seq_t *seq = create_seq();
    char *fname = find_data_file("test.fastq.gz");
    seqfile_t *sf = create_seqfile(fname, "r");
    gzFile fp = gzopen(fname, "r");
    kseq_t *kseq = kseq_init(fp);
    ssize_t kseq_res = 0;
    ssize_t my_res = 0;

    (void) ptr;
    tt_assert(fname != NULL);
    while (1) {
        my_res = read_seqfile(sf, seq);
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
    destroy_seqfile(sf);
    destroy_seq(seq);
    kseq_destroy(kseq);
    gzclose(fp);
    free(fname);
    /* Try again, with fasta */
    seq = create_seq();
    fname = find_data_file("test.fasta");
    tt_assert(fname != NULL);
    sf = create_seqfile(fname, "r");
    fp = gzopen(fname, "r");
    kseq = kseq_init(fp);
    kseq_res = 0;
    my_res = 0;
    while (1) {
        my_res = read_seqfile(sf, seq);
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
    destroy_seqfile(sf);
    destroy_seq(seq);
    kseq_destroy(kseq);
    gzclose(fp);
    if (fname != NULL) {
        free(fname);
    }
}


/*===  FUNCTION  ============================================================*
Name:           test_write_seqfile
Description:    Tests the write_seqfile function from kmseqfile.c
 *===========================================================================*/

void
test_write_seqfile (void *ptr)
{
    seq_t *seq = create_seq();
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
    sf = create_seqfile(fname, "wT");
    seqfile_set_format(sf, FASTQ_FMT);
    res = write_seqfile(sf, seq);
    tt_int_op(res, ==, expt_bytes);
    destroy_seqfile(sf); /* Has to happen here to flush it */
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
    sf = create_seqfile(fname, "wT");
    seqfile_set_format(sf, FASTA_FMT);
    /* do the write */
    res = write_seqfile(sf, seq);
    tt_int_op(res, ==, expt_bytes);
    destroy_seqfile(sf); /* Flush it */
    crc = crc32_file(fname);
    tt_str_op(crc, ==, "0a295c77");
    clean_writable_file(fname);
    fname = NULL;
    /* Check with bad params that it returns -2 */
    res = write_seqfile(NULL, seq);
    tt_int_op(res, ==, -2);
    res = write_seqfile(sf, NULL);
    tt_int_op(res, ==, -2);
end:
    destroy_seqfile(sf);
    destroy_seq(seq);
    if (fname != NULL) free(fname);
    if (crc != NULL) free(crc);
}

struct testcase_t seqfile_tests[] = {
    { "create_seqfile", test_create_seqfile, 0, NULL, NULL},
    { "seqfile_guess_format", test_seqfile_guess_format, 0, NULL, NULL},
    { "destroy_seqfile", test_destroy_seqfile, 0, NULL, NULL},
    { "read_seqfile_vs_kseq", test_read_seqfile_vs_kseq, 0, NULL, NULL},
    { "read_seqfile", test_read_seqfile, 0, NULL, NULL},
    { "write_seqfile", test_write_seqfile, 0, NULL, NULL},
    END_OF_TESTCASES
};
