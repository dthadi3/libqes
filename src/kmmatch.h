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
                size_t len: Compare ``len`` chars. If 0, guess length with
                strlen (may be unsafe).
Description:    Find the hamming distance between two strings. The strings are
                matched until the length of the smallest string.
Returns:        The hamming distance between ``seq1`` and ``seq2``.
 *===========================================================================*/
extern uint_fast32_t hamming(const char *seq1, const char *seq2, size_t len);


/*===  FUNCTION  ============================================================*
Name:           hamming_max
Paramters:      const char *seq1, *seq2: Two strings to compare.
                size_t len: Compare ``len`` chars. If 0, guess length with
                strlen (may be unsafe).
                uint_fast32_t max: Stop counting at ``max``, return ``max + 1``.
Description:    Find the hamming distance between two strings. The strings are
                matched until the length of the smallest string, or ``len``
                charachers, or until the maximum hamming distance (``max``) is
                reached.
Returns:        The hamming distance between ``seq1`` and ``seq2``, or
                ``max + 1`` if the hamming distance exceeds ``max``.
 *===========================================================================*/
extern uint_fast32_t hamming_max(const char *seq1, const char *seq2, size_t len,
        uint_fast32_t max);

#endif /* KMMATCH_H */
