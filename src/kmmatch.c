/*
 * ============================================================================
 *
 *       Filename:  kmmatch.c
 *
 *    Description:  Sequence matching and finding functions used in
 *                  bioinformatic tasks
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */

#include "kmmatch.h"

inline uint_fast32_t
hamming (const char *seq1, const char *seq2, size_t len)
{
    uint_fast32_t mismatches = 0;
    size_t iii = 0;

    /* If we've been given a length of 0, we make it up ourselves */
    if (len == 0) {
        size_t len2 = strlen(seq2);
        len = strlen(seq1);
        /* Max of len & len2 */
        if (len > len2) {
            len = len2;
        }
    }
    /* Count mismatches. See comment on analagous loop in hamming_max for an
       explanation. */
    while(iii < len) {
        if (seq2[iii] != seq1[iii]) {
            mismatches++;
        }
        iii++;
    }
    return mismatches;
}


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
inline uint_fast32_t
hamming_max(const char *seq1, const char *seq2, size_t len, uint_fast32_t max)
{
    uint_fast32_t mismatches = 0;
    size_t iii = 0;

    /* If we've been given a lenght of 0, we make it up ourselves */
    if (len == 0) {
        size_t len2 = strlen(seq2);
        len = strlen(seq1);
        /* Max of len & len2 */
        if (len > len2) {
            len = len2;
        }
    }
    /* We obediently go until ``len``, assuming whoever gave us ``len`` knew
       WTF they were doing. This makes things a bit faster, since these
       functions are expected to be very much inner-loop. */
    while(iii < len) {
        /* Find mismatch count */
        if (seq2[iii] != seq1[iii]) {
            mismatches++;
        }
        iii++;
        if (mismatches > max) {
            /* Bail out if we're over max, always cap at max + 1 */
            return max + 1;
        }
    }
    return mismatches;
}
