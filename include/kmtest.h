/*
 * ============================================================================
 *
 *       Filename:  kmtest.h
 *
 *    Description:  Small testing library
 *
 *        Version:  1.0
 *        Created:  10/11/13 21:41:32
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef KMTEST_H
#define KMTEST_H


/*
TODO:
    - Document usage of this header and make more TEST_X macros or find a way
      to generalise them
*/

#include "kmbase.h"
/* Everyone loves a maze. A lovely macro maze! */
/* Set tab to be 4 spaces or it will look like shit. Sorry Linus */

#define TEST(RETVAL, EXPECTED, NAME, FORMAT)                                  \
    __kmtest_tests++;                                                         \
    if ((RETVAL) != (EXPECTED)) {                                             \
        fprintf(stderr,                                                       \
                "%-*s failed:\n\tgot '" FORMAT "', expected '" FORMAT         \
                "' from '" #RETVAL "'\n",                                     \
                KMLIB_LINEWIDTH-7, NAME, RETVAL, EXPECTED);                   \
        __kmtest_failures++;                                                  \
    }                                                                         \
    else {                                                                    \
        fprintf(stdout, "%-*s passed\n", KMLIB_LINEWIDTH-7, NAME);            \
    }


#define TEST_INT(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%i")
#define TEST_SIZET(RETVAL, EXPECTED, NAME)                                    \
    TEST((size_t)(RETVAL), (size_t)(EXPECTED), NAME, "%zu")
#define TEST_PTR(RETVAL, EXPECTED, NAME)                                      \
    TEST((void *)(RETVAL), (void *)(EXPECTED), NAME, "%p")
#define TEST_PTR_NONNULL(RETVAL, NAME)                                        \
    TEST((void *)RETVAL == NULL, 0, NAME, "%d")
#define TEST_STR(RETVAL, EXPECTED, NAME)                                      \
    __kmtest_tests++;                                                         \
    if (strcmp(RETVAL, EXPECTED) != 0) {                                      \
        fprintf(stderr,                                                       \
                "%-*s failed:\n\tgot '%s', expected '%s' from '" #RETVAL "'\n",\
                KMLIB_LINEWIDTH-7, NAME, RETVAL, EXPECTED);                   \
        __kmtest_failures++;                                                  \
    }                                                                         \
    else {                                                                    \
        fprintf(stdout, "%-*s passed\n", KMLIB_LINEWIDTH-7, NAME);            \
    }




#define __TEST_VARS                                                           \
    static int __kmtest_failures = 0;                                         \
    static int __kmtest_tests = 0;


#ifdef  KMLIB_DEBUG
#define	__TEST_SELFTEST                                                       \
static void test_testers() {                                                  \
    printf("Testing kmtest macros. Every second test should fail\n\n");       \
    /* test TEST macros */                                                    \
                                                                              \
    TEST_INT(1+2, 3, "TEST_INT")                                              \
    TEST_INT(1+2, 5, "TEST_INT")                                              \
                                                                              \
    TEST_SIZET((size_t)1, (size_t)1, "TEST_SIZET")                            \
    TEST_SIZET((size_t)2, (size_t)1, "TEST_SIZET")                            \
                                                                              \
    void *p1 = malloc(1);                                                     \
    void *p2 = malloc(1);                                                     \
    TEST_PTR(p1, p1, "TEST_PTR")                                              \
    TEST_PTR(p1, p2, "TEST_PTR")                                              \
    free(p1); p1=NULL;                                                        \
    free(p2); p2=NULL;                                                        \
                                                                              \
    TEST_STR("test", "test", "TEST_PTR")                                      \
    TEST_STR("test", "", "TEST_PTR")                                          \
                                                                              \
    printf("End of kmtest.h self testing. No more tests should fail\n");      \
    __kmtest_failures -= 4; /* UPDATE THIS NUMBER!!!!!  */                    \
}
#else
#define __TEST_SELFTEST
#endif

#define TEST_INIT()                                                           \
    __TEST_VARS                                                               \
    __TEST_SELFTEST


#define	TEST_SELF() test_testers();

#define	TEST_EXIT()                                                           \
    for (int iii=0; iii<KMLIB_LINEWIDTH; iii++) {printf("%c",'=');}           \
    printf("\n\n");                                                           \
    fprintf(stderr, "Ran %i tests\n", __kmtest_tests);                        \
    if (__kmtest_failures > 0) {                                              \
        fprintf(stderr, "There were %i test failures\n", __kmtest_failures);  \
        exit(EXIT_FAILURE);                                                   \
    } else {                                                                  \
        fprintf(stderr, "All tests passed\n");                                \
    }                                                                         \

#endif /* KMTEST_H */
