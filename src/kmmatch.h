/*
 * ============================================================================
 *
 *       Filename:  kmmatch.h
 *
 *    Description:  Sequence matching and finding functions used in
 *                  bioinformatic tasks
 *
 *        Version:  1.0
 *        Created:  13/11/13 17:38:11
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef KMMATCH_H
#define KMMATCH_H

#include <kmutil.h>


/*===  FUNCTION  ============================================================*
Name:           hamming
Paramters:      const char *seq1, *seq2: Two strings to compare.
Description:    Find the hamming distance between two strings. The strings are
                matched until the length of the smallest string.
Returns:        The hamming distance between ``seq1`` and ``seq2``.
 *===========================================================================*/
extern size_t hamming(const char *seq1, const char *seq2);


/*===  FUNCTION  ============================================================*
Name:           hamming_max
Paramters:      const char *seq1, *seq2: Two strings to compare.
                size_t max: Stop counting at ``max``, return ``max + 1``.
Description:    Find the hamming distance between two strings. The strings are
                matched until the length of the smallest string, or until the
                maximum hamming distance (``max``) is reached.
Returns:        The hamming distance between ``seq1`` and ``seq2``, or
                ``max + 1`` if the hamming distance exceeds ``max``.
 *===========================================================================*/
extern size_t hamming_max(const char *seq1, const char *seq2, size_t max);

#endif /* KMMATCH_H */
