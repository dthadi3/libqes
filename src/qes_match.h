/*
 * ============================================================================
 *
 *       Filename:  qes_match.h
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

#ifndef QES_MATCH_H
#define QES_MATCH_H

#include <qes_util.h>


/*===  FUNCTION  ============================================================*
Name:           qes_match_hamming
Paramters:      const char *seq1, *seq2: Two strings to compare.
                size_t len: Compare ``len`` chars. If 0, guess length with
                strlen (may be unsafe).
Description:    Find the hamming distance between two strings. The strings are
                matched until the length of the smallest string.
Returns:        The hamming distance between ``seq1`` and ``seq2``, or -1 on
                error.
 *===========================================================================*/
extern int_fast32_t qes_match_hamming(const char *seq1, const char *seq2, size_t len);


/*===  FUNCTION  ============================================================*
Name:           qes_match_hamming_max
Paramters:      const char *seq1, *seq2: Two strings to compare.
                size_t len: Compare ``len`` chars. If 0, guess length with
                strlen (may be unsafe).
                int_fast32_t max: Stop counting at ``max``, return ``max + 1``.
Description:    Find the hamming distance between two strings. The strings are
                matched until the length of the smallest string, or ``len``
                charachers, or until the maximum hamming distance (``max``) is
                reached.
Returns:        The hamming distance between ``seq1`` and ``seq2``, or
                ``max + 1`` if the hamming distance exceeds ``max``, or -1 on
                error.
 *===========================================================================*/
extern int_fast32_t qes_match_hamming_max(const char *seq1, const char *seq2, size_t len,
        int_fast32_t max);

#endif /* QES_MATCH_H */
