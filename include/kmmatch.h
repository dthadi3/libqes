/*
 * =====================================================================================
 *
 *       Filename:  kmmatch.h
 *
 *    Description:  Sequence matching and finding functions used in bioinformatic tasks
 *
 *        Version:  1.0
 *        Created:  13/11/13 17:38:11
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au [include word penguin in subject]
 *
 * =====================================================================================
 */
#ifndef KMMATCH_H
#define KMMATCH_H


/* #####   HEADER FILE INCLUDES   ########################################## */

#include <string.h>
#include <stdint.h>


/* #####   EXPORTED FUNCTION DECLARATIONS   ################################ */

extern inline size_t hamming (const char *seq1, const char *seq2);
extern inline size_t hamming_max (const char *seq1, const char *seq2, size_t max);

#endif /* KMMATCH_H */
