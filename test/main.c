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

#include <kmmatch.h>
KMMATCH_INIT()
#include "kmtest.h"


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Run all tests
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
    TEST_INT(calc_hamming("ACTTG", "ACTGG"), 1, "calc_hamming")
    return EXIT_SUCCESS;
}                /* ----------  end of function main  ---------- */
