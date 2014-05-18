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

#include <sys/stat.h>
#include <sys/types.h>

/* TinyTest */
#include "tinytest.h"
#include "tinytest_macros.h"
#include "testdata.h"
#include <kmutil.h>

#define	KM_DEFAULT_ERR_FN errnil
#define KM_EXIT_FN (void)

/* Test replacement functions */

/* Use to avoid calling zfopen except when testing zfopen */

#define	our_zfopen(var, pth, mode) do {             \
    (var) = calloc(1, sizeof(*(var)));              \
    (var)->fp = KM_ZOPEN(pth, mode);                \
    if ((var)->fp == NULL) {                        \
        free((var));                                \
        (var) = NULL;                               \
        break; /* from do {} while,  i.e return*/   \
    }                                               \
    KM_ZBUFFER((var)->fp, KM_FILEBUFFER_LEN);       \
    (var)->eof = 0;                                 \
    (var)->filepos = 0;                             \
    (var)->path = strndup(pth, KM_MAX_FN_LEN);      \
    } while(0)


#define our_create_seqfile(var, fn, mode) do {          \
    (var) = calloc(1, sizeof(seqfile_t));               \
    our_zfopen(((var)->zf), fn, mode);                  \
    if ((var)->zf == NULL) { free((var)); var = NULL; } \
    } while (0)

#define	our_zfclose(f) do { \
    if (f != NULL) {        \
        KM_ZCLOSE(f->fp);   \
        free(f->path);      \
        free(f); f = NULL;  \
    }} while(0)

#define our_destroy_seqfile(sf) do {        \
    if(sf != NULL) {                        \
        our_zfclose(sf->zf);                \
        free(sf);                           \
        sf = NULL;                          \
    }} while (0)

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

/* test_zfile tests */
void test_zfopen(void *);
void test_zfclose(void *);
void test_zfreadline(void *);
void test_zfreadline_realloc(void *);
void test_zfile_guess_mode (void *ptr);
void test_zfpeek (void *ptr);
void test_zfrewind (void *ptr);
static struct testcase_t zfile_tests[] = {
    { "zfopen", test_zfopen,},
    { "zfpeek", test_zfpeek,},
    { "zfreadline", test_zfreadline,},
    { "zfreadline_realloc", test_zfreadline_realloc,},
    { "zfile_guess_mode", test_zfile_guess_mode, },
    { "zfclose", test_zfclose,},
    { "zfrewind", test_zfrewind,},
    END_OF_TESTCASES
};

/* test_seqfile tests */
void test_create_seqfile (void *);
void test_seqfile_guess_format (void *ptr);
void test_destroy_seqfile (void *ptr);
static struct testcase_t seqfile_tests[] = {
    { "create_seqfile", test_create_seqfile,},
    { "seqfile_guess_format", test_seqfile_guess_format,},
    { "destroy_seqfile", test_destroy_seqfile,},
    END_OF_TESTCASES
};

#endif /* TESTS_H */
