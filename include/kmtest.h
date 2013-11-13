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

#define TEST(RETVAL, EXPECTED, NAME, FORMAT)                                                    \
    if (RETVAL != EXPECTED) {                                                                   \
        fprintf(stderr,                                                                         \
                "%-72s failed:\n\tgot '" FORMAT "', expected '" FORMAT "' from '" #RETVAL "'\n",\
                NAME, RETVAL, EXPECTED);                                                        \
        __kmtest_failures++;                                                                    \
    }                                                                                           \
    else {                                                                                      \
        fprintf(stdout, "%-72s passed\n", NAME);                                                \
    }


#define TEST_INT(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%i")
#define TEST_SIZET(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%zu")
#define TEST_PTR(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%p")
#define TEST_STR(RETVAL, EXPECTED, NAME) TEST(RETVAL, EXPECTED, NAME, "%s")


#define TEST_INIT()                                                                             \
static int __kmtest_failures = 0;                                                               \
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
}                                                                                               \


#define	TEST_SELF() test_testers();

#define	TEST_EXIT()                                                                             \
   if (__kmtest_failures > 0) {                                                                 \
       fprintf(stderr, "There were %i test failures\n");                                        \
       exit(EXIT_FAILURE);                                                                      \
   } else {                                                                                     \
       fprintf(stderr, "All tests passed\n");                                                   \
   }                                                                                            \

#endif /* KMTEST_H */
