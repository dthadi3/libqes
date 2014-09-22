/*
 * ============================================================================
 *
 *       Filename:  tests.h
 *
 *    Description:  Tests for libqes
 *
 *        Version:  1.0
 *        Created:  04/05/14 15:48:16
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

/* TinyTest */
#include "tinytest.h"
#include "tinytest_macros.h"
#include "testdata.h"
#include "helpers.h"

#define	QES_DEFAULT_ERR_FN errnil
#define QES_EXIT_FN (void)
#include <qes_util.h>

/* List of tests format is:
   { name, fn, flags, testcase_setup_t *ptr, void * for testcase_setup_t }
 */
/* test_util tests */
void test_qes_calloc(void *);
void test_qes_malloc(void *);
void test_qes_realloc(void *);
void test_qes_free(void *);
void test_qes_roundup32(void *);
void test_qes_roundup64(void *);
void test_expects (void *);
extern struct testcase_t qes_util_tests[];

/* test_match tests */
void test_qes_hamming(void *);
void test_qes_hamming_max(void *);
extern struct testcase_t qes_match_tests[];

/* test_qes_fil tests */
void test_qes_file_open(void *);
void test_qes_file_close(void *);
void test_qes_file_readline(void *);
void test_qes_file_readline_realloc(void *);
void test_qes_file_guess_mode (void *);
void test_qes_file_peek (void *);
void test_qes_file_rewind (void *);
void test_qes_file_getuntil (void *);
void test_qes_file_ok (void *);
extern struct testcase_t qes_file_tests[];

/* test_seqfile tests */
void test_qes_seqfile_guess_format (void *);
void test_qes_seqfile_create(void *);
void test_qes_seqfile_destroy (void *);
void test_qes_seqfile_read (void *);
void test_qes_seqfile_read_vs_kseq (void *);
void test_qes_seqfile_write (void *);
extern struct testcase_t qes_seqfile_tests[];

/* test_seq tests */
void test_qes_seq_create (void *);
void test_qes_seq_create_no_qual (void *);
void test_qes_seq_create_no_qual_or_comment (void *);
void test_qes_seq_ok (void *);
void test_qes_seq_ok_no_comment (void *);
void test_qes_seq_ok_no_qual (void *);
void test_qes_seq_ok_no_comment_or_qual (void *);
void test_qes_seq_destroy (void *);
void test_qes_seq_fill_funcs(void *);
void test_qes_seq_copy(void *);
extern struct testcase_t qes_seq_tests[];

/* test_sequtil tests */
void test_qes_sequtil_translate_codon (void *);
extern struct testcase_t qes_sequtil_tests[];

#endif /* TESTS_H */
