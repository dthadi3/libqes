/*
 * ============================================================================
 *
 *       Filename:  test_match.c
 *
 *    Description:  Test kmmatch functions
 *
 *        Version:  1.0
 *        Created:  04/05/14 15:52:13
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"

#include <kmmatch.h>

void
test_hamming (void *p)
{
    (void) (p);
    tt_int_op(hamming("ACTTG", "ACTGG"), ==, 1);
    tt_int_op(hamming("ACTTG", "ACTGGA"), ==, SIZE_MAX);
    tt_int_op(hamming("ACTTG", "ACTTG"), ==, 0);
end:
    ;
}


void
test_hamming_max (void *p)
{
    (void) (p);
    tt_int_op(hamming_max("ACTTG", "ACTGG", 1), ==, 1);
    tt_int_op(hamming_max("ACTTG", "ACTGG", 0), ==, 0);
    tt_int_op(hamming_max("ACTTG", "ACTGGA", 1), ==, SIZE_MAX);
    tt_int_op(hamming_max("ACTTG", "ACTTG", 1), ==, 0);
end:
    ;
}

struct testcase_t match_tests[] = {
    { "hamming", test_hamming, 0, NULL, NULL},
    { "hamming_max", test_hamming_max, 0, NULL, NULL},
    END_OF_TESTCASES
};
