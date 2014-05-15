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
 *       Compiler:  gcc
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

/* TinyTest */
#include "tinytest.h"
#include "tinytest_macros.h"
#include "testdata.h"

#define	KM_DEFAULT_ERR_FN errnil
#define KM_EXIT_FN (void)

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
static struct testcase_t util_tests[] = {
    { "km_calloc", test_km_calloc,},
    { "km_malloc", test_km_malloc,},
    { "km_realloc", test_km_realloc,},
    { "km_free", test_km_free,},
    { "kmroundup32", test_kmroundup32,},
    { "kmroundup64", test_kmroundup64,},
    END_OF_TESTCASES
};

/* test_match tests */
void test_hamming(void *);
void test_hamming_max(void *);
static struct testcase_t match_tests[] = {
    { "hamming", test_hamming,},
    { "hamming_max", test_hamming_max,},
    END_OF_TESTCASES
};
#endif /* TESTS_H */
