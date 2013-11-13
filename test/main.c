/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Tests for kmlib
 *
 *        Version:  1.0
 *        Created:  11/11/13 21:36:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include "kmlib.h"
#include "kmtest.h"
TEST_INIT()

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Run all tests
 * =====================================================================================
 */

static int test_kmseq_creation(const char * filename) {

    kmfile *file = create_kmfile(filename);

#ifdef  KMLIB_DEBUG
    printf("kmfile at %x\n", file);
#endif     /* -----  not KMLIB_DEBUG  ----- */

    destroy_kmfile(file);
    file = NULL;

    return 0;

}
int
main ( int argc, char *argv[] )
{
#ifdef  KMLIB_DEBUG
    TEST_SELF();
#endif     /* -----  not KMLIB_DEBUG  ----- */

    /*  KMMATCH unit tests */

    TEST_SIZET(hamming("ACTTG", "ACTGG"), (size_t)1, "hamming");
    TEST_SIZET(hamming("ACTTG", "ACTGGA"), SIZE_MAX, "hamming seqlen not equal");

    TEST_SIZET(hamming_max("ACTTG", "ACTGG", 1), (size_t)1, "hamming_max with max > hamming");
    TEST_SIZET(hamming_max("ACTTG", "ACTGG", 0), (size_t)0, "hamming_max with max < hamming");
    TEST_SIZET(hamming_max("ACTTG", "ACTGGA", 1), SIZE_MAX, "hamming_max seqlen not equal");

    TEST_INT(test_kmseq_creation("./test/data/test.fastq"), 0, "test_kmseq_creation");

    TEST_EXIT();
    return EXIT_SUCCESS;
}                /* ----------  end of function main  ---------- */
