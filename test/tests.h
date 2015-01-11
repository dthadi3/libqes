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
extern struct testcase_t qes_util_tests[];
/* test_match tests */
extern struct testcase_t qes_match_tests[];
/* test_qes_file tests */
extern struct testcase_t qes_file_tests[];
/* test_seqfile tests */
extern struct testcase_t qes_seqfile_tests[];
/* test_seq tests */
extern struct testcase_t qes_seq_tests[];
/* test_sequtil tests */
extern struct testcase_t qes_sequtil_tests[];
/* test_log tests */
extern struct testcase_t qes_log_tests[];

#endif /* TESTS_H */
