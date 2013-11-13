#ifndef KMTEST_H
#define KMTEST_H

/*
 * =====================================================================================
 *
 *       Filename:  kmtest.h
 *
 *    Description:  Small testing library
 *
 *        Version:  1.0
 *        Created:  10/11/13 21:41:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */


#include "kmbase.h"

#define TEST(RETVAL, EXPECTED, NAME, FORMAT)                                                    \
    __kmtest_tests++;                                                                           \
    if (RETVAL != EXPECTED) {                                                                   \
        fprintf(stderr,                                                                         \
                "%-*s failed:\n\tgot '" FORMAT "', expected '" FORMAT "' from '" #RETVAL "'\n", \
                KMLIB_LINEWIDTH-7, NAME, RETVAL, EXPECTED);                                     \
        __kmtest_failures++;                                                                    \
    }                                                                                           \
    else {                                                                                      \
        fprintf(stdout, "%-*s passed\n", KMLIB_LINEWIDTH-7, NAME);                              \
    }


#define TEST_INT(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%i")
#define TEST_SIZET(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%zu")
#define TEST_PTR(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%p")
#define TEST_STR(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%s")


#define __TEST_VARS                                                                             \
    static int __kmtest_failures = 0;                                                           \
    static int __kmtest_tests = 0;                                                              \


#ifdef  KMLIB_DEBUG
#define	__TEST_SELFTEST                                                                       \
static void test_testers() {                                                                    \
    printf("Testing kmtest macros. Every second test should fail\n\n");                         \
    /* test TEST macros */                                                                      \
                                                                                                \
    TEST_INT(1+2, 3, "TEST_INT")                                                                \
    TEST_INT(1+2, 5, "TEST_INT")                                                                \
                                                                                                \
    TEST_SIZET((size_t)1, (size_t)1, "TEST_SIZET")                                              \
    TEST_SIZET((size_t)2, (size_t)1, "TEST_SIZET")                                              \
                                                                                                \
    TEST_PTR(&"test", &"test", "TEST_PTR")                                                      \
    TEST_PTR(&"test", NULL, "TEST_PTR")                                                         \
                                                                                                \
    TEST_PTR(&"test", &"test", "TEST_PTR")                                                      \
    TEST_PTR(&"test", NULL, "TEST_PTR")                                                         \
                                                                                                \
    printf("End of kmtest.h self testing. No more tests should fail\n");                        \
    __kmtest_failures -= 4; /* UPDATE THIS NUMBER!!!!!  */                                      \
}
#else
#define __TEST_SELFTEST
#endif

#define TEST_INIT()                                                                             \
    __TEST_VARS                                                                                 \
    __TEST_SELFTEST


#define	TEST_SELF() test_testers();

#define	TEST_EXIT()                                                                             \
    for (int iii=0; iii<KMLIB_LINEWIDTH; iii++) {printf("%c",'=');}                             \
    printf("\n\n");                                                                             \
    fprintf(stderr, "Ran %i tests\n", __kmtest_tests);                                          \
    if (__kmtest_failures > 0) {                                                                \
        fprintf(stderr, "There were %i test failures\n", __kmtest_failures);                    \
        exit(EXIT_FAILURE);                                                                     \
    } else {                                                                                    \
        fprintf(stderr, "All tests passed\n");                                                  \
    }                                                                                           \

#endif /* KMTEST_H */
