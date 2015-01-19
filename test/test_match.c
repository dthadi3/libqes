/*
 * ============================================================================
 *
 *       Filename:  test_match.c
 *
 *    Description:  Test qes_match functions
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"
#include <qes_match.h>
#include <limits.h>


static void
test_qes_hamming (void *p)
{
    (void) (p);
    /* Simple stuff */
    tt_int_op(qes_match_hamming("ACTTG", "ACTTG", 5), ==, 0);
    tt_int_op(qes_match_hamming("ACTTG", "ACTGG", 5), ==, 1);
    /* Different lengths */
    tt_int_op(qes_match_hamming("ACTTGA", "ACTTG", 5), ==, 0);
    tt_int_op(qes_match_hamming("ACTTG", "ACTTGA", 5), ==, 0);
    tt_int_op(qes_match_hamming("ACTTG", "ACTGGA", 5), ==, 1);
    tt_int_op(qes_match_hamming("ACTTG", "ACTGGA", 6), ==, 2);
    tt_int_op(qes_match_hamming("ACTTG", "ACTTGA", 6), ==, 1);
    /* Make it guess lengths */
    tt_int_op(qes_match_hamming("ACTTG", "ACTTG", 0), ==, 0);
    tt_int_op(qes_match_hamming("ACATG", "ACTTG", 0), ==, 1);
    tt_int_op(qes_match_hamming("ACTTG", "ACTTGT", 0), ==, 0);
    tt_int_op(qes_match_hamming("ACATG", "ACTTGT", 0), ==, 1);
    tt_int_op(qes_match_hamming("ACTTGT", "ACTTG", 0), ==, 0);
    /* Give it hell */
    tt_int_op(qes_match_hamming("ACTTG", NULL, 0), ==, -1);
    tt_int_op(qes_match_hamming(NULL, "ACTTG", 0), ==, -1);
    tt_int_op(qes_match_hamming(NULL, NULL, 0), ==, -1);
end:
    ;
}


static void
test_qes_hamming_max (void *p)
{

    (void) (p);
    /* Same tests as per hamming, max is INT_MAX */
    /* Simple stuff */
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTTG", 5, INT_MAX), ==, 0);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGG", 5, INT_MAX), ==, 1);
    /* Different lengths */
    tt_int_op(qes_match_hamming_max("ACTTGA", "ACTTG", 5, INT_MAX), ==, 0);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTTGA", 5, INT_MAX), ==, 0);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGGA", 5, INT_MAX), ==, 1);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGGA", 6, INT_MAX), ==, 2);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTTGA", 6, INT_MAX), ==, 1);
    /* Make it guess lengths */
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTTG", 0, INT_MAX), ==, 0);
    tt_int_op(qes_match_hamming_max("ACATG", "ACTTG", 0, INT_MAX), ==, 1);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTTGT", 0, INT_MAX), ==, 0);
    tt_int_op(qes_match_hamming_max("ACATG", "ACTTGT", 0, INT_MAX), ==, 1);
    tt_int_op(qes_match_hamming_max("ACTTGT", "ACTTG", 0, INT_MAX), ==, 0);
    /* Test it bails out when over ``max`` */
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGG", 5, 1), ==, 1);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGG", 5, 0), ==, 1);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGA", 5, 0), ==, 1);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGA", 5, 1), ==, 2);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTGA", 5, 2), ==, 2);
    /* Give it hell */
    tt_int_op(qes_match_hamming_max("ACTTG", NULL, 0, INT_MAX), ==, -1);
    tt_int_op(qes_match_hamming_max(NULL, "ACTTG", 0, INT_MAX), ==, -1);
    tt_int_op(qes_match_hamming_max(NULL, NULL, 0, INT_MAX), ==, -1);
    tt_int_op(qes_match_hamming_max("ACTTG", "ACTTG", 0, -1), ==, -1);
    tt_int_op(qes_match_hamming_max("ACTTG", NULL, 0, -1), ==, -1);
    tt_int_op(qes_match_hamming_max(NULL, "ACTTG", 0, -1), ==, -1);
    tt_int_op(qes_match_hamming_max(NULL, NULL, 0, -1), ==, -1);
end:
    ;
}

struct testcase_t qes_match_tests[] = {
    { "qes_match_hamming", test_qes_hamming, 0, NULL, NULL},
    { "qes_match_hamming_max", test_qes_hamming_max, 0, NULL, NULL},
    END_OF_TESTCASES
};
