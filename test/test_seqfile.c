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
test_create_seqfile(void *p)
{
    seqfile_t *sf = NULL;
    char writeable[KM_MAX_FN_LEN];
    int filenm = 0;
    (void) p;
    /* test opening a valid, unziped FASTQ */
    sf = create_seqfile(fastq_file, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_READ);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    destroy_seqfile(sf);
    /* Test opening a file in transparent write mode */
    snprintf(writeable, KM_MAX_FN_LEN, "%s/%s_%d.file", out_prefix, __func__,
            filenm++);
    sf = create_seqfile(writeable, "wT");
    KM_ZWRITE(sf->zf->fp, "ABCD", 4);
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    tt_int_op(access(writeable, F_OK), ==, 0);
    destroy_seqfile(sf);
    remove(writeable);
    /* Test opening a file in write mode , acually zipping output */
    snprintf(writeable, KM_MAX_FN_LEN, "%s/%s_%d.file", out_prefix, __func__,
            filenm++);
    sf = create_seqfile(writeable, "w9");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    destroy_seqfile(sf);
    tt_int_op(access(writeable, F_OK), ==, 0);
    remove(writeable);
end:
    destroy_seqfile(sf);
    ;
}

void
test_seqfile_guess_format (void *ptr)
{
    seqfile_t *sf = NULL;
    int res = -1;
    (void) ptr;
    /* test with a FASTQ file */
    sf = create_seqfile(fastq_file, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    destroy_seqfile(sf);
    /* test with a FASTA file */
    sf = create_seqfile(fasta_file, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTA_FMT);
    tt_int_op(sf->flags.format, ==, FASTA_FMT);
    destroy_seqfile(sf);
    /* test with a gziped FASTQ file */
    sf = create_seqfile(gzfastq_file, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    destroy_seqfile(sf);
end:
    destroy_seqfile(sf);
}

void
test_destroy_seqfile (void *ptr)
{
    seqfile_t *sf = NULL;
    sf = create_seqfile(fastq_file, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_assert(seqfile_ok(sf));
    destroy_seqfile(sf);
    tt_ptr_op(sf, ==, NULL);
    tt_assert(!seqfile_ok(sf));
end:
    destroy_seqfile(sf);
}

void
test_read_seqfile_vs_kseq (void *ptr)
{
    seq_t *seq = create_seq();
    seqfile_t *sf = create_seqfile(gzfastq_file, "r");
    gzFile fp = gzopen(gzfastq_file, "r");
    kseq_t *kseq = kseq_init(fp);
    ssize_t kseq_res = 0;
    ssize_t my_res = 0;
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
    /* Try again, with fasta */
    seq = create_seq();
    sf = create_seqfile(fasta_file, "r");
    fp = gzopen(fasta_file, "r");
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
}


struct testcase_t seqfile_tests[] = {
    { "create_seqfile", test_create_seqfile,},
    { "seqfile_guess_format", test_seqfile_guess_format,},
    { "destroy_seqfile", test_destroy_seqfile,},
    { "read_seqfile_vs_kseq", test_read_seqfile_vs_kseq,},
    END_OF_TESTCASES
};
