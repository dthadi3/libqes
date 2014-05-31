/*
 * ============================================================================
 *
 *       Filename:  tests.h
 *
 *    Description:  Tests for kmlib
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

#define	KM_DEFAULT_ERR_FN errnil
#define KM_EXIT_FN (void)
#include <kmutil.h>

/* List of tests format is:
   { name, fn, flags, testcase_setup_t *ptr, void * for testcase_setup_t }
 */
/* test_util tests */
void test_km_calloc(void *);
void test_km_malloc(void *);
void test_km_realloc(void *);
void test_km_free(void *);
void test_kmroundup32(void *);
void test_kmroundup64(void *);
extern struct testcase_t util_tests[];

/* test_match tests */
void test_hamming(void *);
void test_hamming_max(void *);
extern struct testcase_t match_tests[];

/* test_zfile tests */
void test_zfopen(void *);
void test_zfclose(void *);
void test_zfreadline(void *);
void test_zfreadline_realloc(void *);
void test_zfile_guess_mode (void *);
void test_zfpeek (void *);
void test_zfrewind (void *);
void test_zfgetuntil (void *);
void test_zfile_ok (void *);
extern struct testcase_t zfile_tests[];

/* test_seqfile tests */
void test_create_seqfile (void *);
void test_seqfile_guess_format (void *);
void test_destroy_seqfile (void *);
void test_read_seqfile_vs_kseq (void *);
void test_read_seqfile (void *);
extern struct testcase_t seqfile_tests[];

/* test_seq tests */
void test_create_seq (void *);
void test_create_seq (void *);
void test_create_seq_no_qual (void *);
void test_create_seq_no_qual_or_comment (void *);
void test_seq_ok (void *);
void test_seq_ok_no_comment (void *);
void test_seq_ok_no_qual (void *);
void test_seq_ok_no_comment_or_qual (void *);
void test_destroy_seq (void *);
extern struct testcase_t seq_tests[];

#endif /* TESTS_H */
