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
#define KM_EXIT_FN (void)
#include "kmutil.h"

/* Constants */
static const size_t bufsize = 1<<10;
static const unsigned char *zeros[1<<10];
static const char *test_data_dir = "." km_pathsep "data";

/* Helper functions */
char *
get_test_filename (const char* file)
{
    char * ret = calloc(bufsize, sizeof(*ret));
    int len = snprintf(ret, bufsize, "%s" km_pathsep "%s", test_data_dir, file);
    if (len > 0 && len < bufsize) return ret;
    else return NULL;
}

static const char *km_test_err_msg = NULL;

static void
test_err_handler(const char *msg,  const char *f, int l, ...)
{
    km_test_err_msg = msg;
    (void) (f);
    (void) (l);
}


/* Actual tests */
void
test_km_calloc(void *ptr)
{
    void *res = NULL;
    bzero(zeros, bufsize);
    /* This should work, and the buffer should be zero throughout */
    res = km_calloc(1, 1, &errnil);
    tt_ptr_op(res, !=, NULL);
    tt_int_op(memcmp(res, zeros, 1), ==, 0);
    free(res);
    /* This should fail */
    res = km_calloc(SIZE_MAX, 1, &test_err_handler);
    tt_ptr_op(res, ==, NULL);

end:
    if (res != NULL) free(res);
    ;
}

void
test_km_malloc(void *ptr)
{
    void *res = NULL;

    res = km_malloc(1, &errnil);
    tt_ptr_op(res, !=, NULL);
    free(res);

    /* This should fail */
    res = km_malloc(SIZE_MAX, &test_err_handler);
    tt_ptr_op(res, ==, NULL);

end:
    if (res != NULL) free(res);
    ;
}

void
test_km_realloc(void *ptr)
{
    char *res = NULL;
    const char *str = "test";
    char *dat = strdup(str);
    /* Test resizing buffer */
    res = km_realloc(dat, 10, &errnil);
    tt_ptr_op(res, !=, NULL);
    tt_int_op(memcmp(res, str, 5), ==, 0);
    free(res);
    /* This should fail */
    res = km_realloc(dat, SIZE_MAX, &test_err_handler);
    tt_ptr_op(res, ==, NULL);
end:
    if (res != NULL) free(res);
    ;
}

void
test_km_free(void *ptr)
{
    char *dat = strdup("test");
    /* Test freeing buffer */
    tt_ptr_op(dat, !=, NULL);
    km_free(dat);
    tt_ptr_op(dat, ==, NULL);
    /* This free(NULL) should not fail */
    km_free(dat);
    tt_ptr_op(dat, ==, NULL);
end:
    ;
}

void
test_kmroundup32 (void *ptr)
{
    int32_t val = 3;
    tt_int_op(kmroundup32(val), ==, 4);
    val++;
    tt_int_op(kmroundup32(val), ==, 8);
    val = 8;
    tt_int_op(kmroundup32(val), ==, 16);
    val = 262143;
    tt_int_op(kmroundup32(val), ==, 262144);
    /* Bored now, lets assume it works until: */
    val = (1<<30) + 1;
    tt_int_op(kmroundup32(val), ==, -2147483648); /* It's signed, remember. Overflows */
    uint32_t uval = (1u<<31) - 1;
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
    int64_t res = kmroundup64(val);
    tt_int_op(res, ==, 4);
    val++;
    tt_int_op(kmroundup64(val), ==, 8);
    val = 8;
    tt_int_op(kmroundup64(val), ==, 16);
    val = 262143llu;
    tt_int_op(kmroundup64(val), ==, 262144);
    /* Bored now, lets assume it works until: */
    val = (1llu<<62) + 1;
    tt_int_op(kmroundup64(val), ==, -9223372036854775808llu); /* It's signed, remember. Overflows */
    uint64_t uval = (1llu<<63) - 1;
    tt_int_op(kmroundup64(uval), ==, 1llu<<63);
    uval = 1llu<<62;
    tt_int_op(kmroundup64(uval), ==, 1llu<<63);
    uval = 63llu;
    tt_int_op(kmroundup64(uval) - 2, ==, 62llu);
end:
    ;
}

