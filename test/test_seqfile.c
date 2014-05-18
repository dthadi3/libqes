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
    /* Test opening a file in transparent write mode */
    snprintf(writeable, KM_MAX_FN_LEN, "%s/%s_%d.file", out_prefix, __func__,
            filenm++);
    sf = create_seqfile(writeable, "wT");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    our_destroy_seqfile(sf);
    tt_int_op(access(writeable, F_OK), ==, 0);
    remove(writeable);
    /* Test opening a file in write mode , acually zipping output */
    snprintf(writeable, KM_MAX_FN_LEN, "%s/%s_%d.file", out_prefix, __func__,
            filenm++);
    sf = create_seqfile(writeable, "w9");
    tt_ptr_op(sf, !=, NULL);
    tt_ptr_op(sf->zf, !=, NULL);
    tt_int_op(sf->zf->mode, ==, RW_WRITE);
    tt_int_op(sf->n_records, ==, 0);
    our_destroy_seqfile(sf);
    tt_int_op(access(writeable, F_OK), ==, 0);
    remove(writeable);
end:
    our_destroy_seqfile(sf);
    ;
}

void
test_seqfile_guess_format (void *ptr)
{
    seqfile_t *sf = NULL;
    int res = -1;
    (void) ptr;
    /* test with a FASTQ file */
    our_create_seqfile(sf, fastq_file, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    our_destroy_seqfile(sf);
    /* test with a FASTA file */
    our_create_seqfile(sf, fasta_file, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTA_FMT);
    tt_int_op(sf->flags.format, ==, FASTA_FMT);
    our_destroy_seqfile(sf);
    /* test with a gziped FASTQ file */
    our_create_seqfile(sf, gzfastq_file, "r");
    res = seqfile_guess_format(sf);
    tt_int_op(res, ==, FASTQ_FMT);
    tt_int_op(sf->flags.format, ==, FASTQ_FMT);
    our_destroy_seqfile(sf);
end:
    our_destroy_seqfile(sf);
}

void
test_destroy_seqfile (void *ptr)
{
    seqfile_t *sf = NULL;
    our_create_seqfile(sf, fastq_file, "r");
    tt_ptr_op(sf, !=, NULL);
    tt_assert(seqfile_ok(sf));
    destroy_seqfile(sf);
    tt_ptr_op(sf, ==, NULL);
    tt_assert(!seqfile_ok(sf));
end:
    our_destroy_seqfile(sf);

}
