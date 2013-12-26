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
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */

#include "kmmatch.h"

/*
 * ===  FUNCTION  =============================================================
 *         Name:    hamming_max
 *  Description:    Calculates the hamming distance up until max. Useful for
 *                      non-exact substring finding
 * Return Value:    size_t: max if max >= hamming dist, else hamming dist
 * ============================================================================
 */

inline size_t
hamming_max                    (const char     *seq1,
                                const char     *seq2,
                                size_t          max)
{
    size_t      len = strlen(seq1);

    /* check seq lengths */
    if (len != strlen(seq2)) {
#ifdef  KMLIB_DEBUG
        KM_ERROR("length of seq1 and seq2 not the same")
        fprintf(stderr, "\tseq1:\t'%s'\n", seq1);
        fprintf(stderr, "\tseq2:\t'%s'\n", seq2);
        fprintf(stderr, "\tmax:\t'%zu'\n", max);
#endif     /* -----  not FDB_DEBUG  ----- */
        return(SIZE_MAX);
    }

    if (strncmp(seq2, seq1, len) == 0){
        /* exact match */
        return 0;
    } else {
        size_t mismatches = 0;
        size_t iii = 0;
        while(iii < len && mismatches < max) {
            if (seq2[iii] != seq1[iii]) {
                mismatches++;
            }
            iii++;
        }
        return mismatches;
    }
}



/*
 * ===  FUNCTION  =============================================================
 *         Name:    hamming
 *  Description:    Calculate Hamming distance between two same-lengthed
 *                      strings
 * Return Value:    size_t: Hamming distance, i.e. number of differences
 *                      between strings. 0 < hamming < strlen(seq1)
 * ============================================================================
 */

inline size_t
hamming            (const char     *seq1,
                    const char     *seq2)
{
    size_t      len = strlen(seq1);

    /* check seq lengths */
    if (len != strlen(seq2)) {
#ifdef  KMLIB_DEBUG
        KM_ERROR("length of seq1 and seq2 not the same")
        fprintf(stderr, "\tseq1:\t'%s'\n", seq1);
        fprintf(stderr, "\tseq2:\t'%s'\n", seq2);
#endif     /* -----  not FDB_DEBUG  ----- */
        return(SIZE_MAX);
    }

    if (strncmp(seq2, seq1, len) == 0){
        /* exact match */
        return 0;
    } else {
        size_t mismatches = 0;
        for (size_t iii = 0; iii < len; iii++) {
            if (seq2[iii] != seq1[iii]) {
                mismatches++;
            }
        }
        return mismatches;
    }
}
