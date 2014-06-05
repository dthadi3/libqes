/*
 * ============================================================================
 *
 *       Filename:  test_util.c
 *
 *    Description:  Test kmutil.c
 *
 *        Version:  1.0
 *        Created:  05/03/14 21:44:03
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc / clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#include "tests.h"
#include <kmutil.h>


/* Actual tests */
void
test_km_calloc(void *ptr)
{
    void *res = NULL;
    const size_t bufsize = 1<<10;
    unsigned char *zeros[1<<10];

    (void) ptr;
    bzero(zeros, bufsize);
    /* This should work, and the buffer should be zero throughout */
    res = km_calloc(1, 1);
    tt_ptr_op(res, !=, NULL);
    tt_int_op(memcmp(res, zeros, 1), ==, 0);
    free(res);
    res = NULL;
    /* This should fail */
    res = km_calloc(SIZE_MAX, 1);
    tt_ptr_op(res, ==, NULL);
end:
    if (res != NULL) free(res);
}

void
test_km_malloc(void *ptr)
{
    void *res = NULL;

    (void) ptr;
    res = km_malloc(1);
    tt_ptr_op(res, !=, NULL);
    free(res);
    res = NULL;
    /* This should fail */
    res = km_malloc(SIZE_MAX);
    tt_ptr_op(res, ==, NULL);
end:
    if (res != NULL) free(res);
}

void
test_km_realloc(void *ptr)
{
    char *res = NULL;
    const char *str = "test";
    char *dat = strdup(str);

    (void) ptr;
    /* Test resizing buffer */
    res = km_realloc(dat, 10);
    dat = NULL;
    tt_ptr_op(res, !=, NULL);
    tt_int_op(memcmp(res, str, 5), ==, 0);
    free(res);
    res = NULL;
    /* This should fail */
    res = km_realloc(dat, SIZE_MAX);
    tt_ptr_op(res, ==, NULL);
end:
    if (res != NULL) free(res);
    if (dat != NULL) free(dat);
}

void
test_km_free(void *ptr)
{
    char *dat = strdup("test");

    (void) ptr;
    /* Test freeing buffer */
    tt_ptr_op(dat, !=, NULL);
    km_free(dat);
    tt_ptr_op(dat, ==, NULL);
    /* This free(NULL) should not fail */
    km_free(dat);
    tt_ptr_op(dat, ==, NULL);
end:
    if (dat != NULL) free(dat);
}

void
test_kmroundup32 (void *ptr)
{
    int32_t val = 3;
    uint32_t uval = (1u<<31) - 1;

    (void) ptr;
    /* Signed */
    tt_int_op(kmroundup32(val), ==, 4);
    val++;
    tt_int_op(kmroundup32(val), ==, 8);
    val = 8;
    tt_int_op(kmroundup32(val), ==, 16);
    val = 262143;
    tt_int_op(kmroundup32(val), ==, 262144);
    /* Unsigned */
    tt_int_op(kmroundup32(uval), ==, 1u<<31);
    uval++;
    tt_int_op(kmroundup32(uval), ==, 0);
end:
    ;
}

void
test_kmroundup64 (void *ptr)
{
    int64_t val = 3;
    uint64_t uval = (1llu<<63) - 1;

    (void) ptr;
    /* Signed */
    tt_int_op(kmroundup64(val), ==, 4);
    val = 4;
    tt_int_op(kmroundup64(val), ==, 8);
    val = 8;
    tt_int_op(kmroundup64(val), ==, 16);
    val = 262143llu;
    tt_int_op(kmroundup64(val), ==, 262144);
    /* Unsigned */
    tt_int_op(kmroundup64(uval), ==, 1llu<<63);
    uval = 1llu<<62;
    tt_int_op(kmroundup64(uval), ==, 1llu<<63);
    uval = 63llu;
    tt_int_op(kmroundup64(uval) - 2, ==, 62llu);
end:
    ;
}

void
test_expects (void *ptr)
{
    int t = 1;
    int f = 0;

    (void) ptr;
    tt_assert(km_likely(t));
    tt_assert(!km_likely(f));
    tt_assert(km_unlikely(t));
    tt_assert(!km_unlikely(f));
end:
    ;
}

struct testcase_t util_tests[] = {
    { "km_calloc", test_km_calloc, 0, NULL, NULL},
    { "km_malloc", test_km_malloc, 0, NULL, NULL},
    { "km_realloc", test_km_realloc, 0, NULL, NULL},
    { "km_free", test_km_free, 0, NULL, NULL},
    { "kmroundup32", test_kmroundup32, 0, NULL, NULL},
    { "kmroundup64", test_kmroundup64, 0, NULL, NULL},
    { "expects", test_expects, 0, NULL, NULL},
    END_OF_TESTCASES
};
