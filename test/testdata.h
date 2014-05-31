/*
 * ============================================================================
 *
 *       Filename:  testdata.h
 *
 *    Description:  Data for tests
 *
 *        Version:  1.0
 *        Created:  17/03/14 16:26:59
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef TESTDATA_H
#define TESTDATA_H

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
#include "helpers.h"


extern const size_t n_loremipsum_lines;
extern const size_t loremipsum_fsize;
extern const size_t loremipsum_line_lens[];
extern const char *loremipsum_lines[];
extern const char *first_fastq_read[];
extern const size_t first_fastq_len;
extern struct testcase_t data_tests[];

#endif /* TESTDATA_H */
