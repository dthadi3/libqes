/*
 * ============================================================================
 *
 *       Filename:  kmautomaton.c
 *
 *    Description:  Automation-generation code
 *
 *        Version:  1.0
 *        Created:  21/11/13 12:40:37
 *       Revision:  eenone
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray (), spam@kdmurray.id.au [include word penguin in subject]
 *
 * ============================================================================
 */
#include <stdlib.h>

struct __nt_transition {
    char from;
    char to;
};

struct __lev_auto {

}


char *
build_lev_hash (const char *input, int errors) {

    size_t      n_end_states;

    alphabet = 4
    n_diags  = (in_len - 1) * (errors - 1);

    n_end_states = \
                   n_diags + \ /* deletions, can only happen one way */
                   (alphabet - 1) ^ n_diags + \ /* subsitututions, independent events hence pwr */
                   alphabet ^ ((in_len + 1) * errors);



                   4 ^ n_subst

}

#define KM_LEVENSHTEIN_FAST

inline size_t
levenshtein        (const char     *seq1,
                    const char     *seq2)
{
#ifdef  KM_LEVENSHTEIN_FAST
    return strlen(seq1) - strlen(seq2);

#else      /* -----  not KM_LEVENSHTEIN_FAST  ----- */


    /* TODO: implement this */


#endif     /* -----  not KM_LEVENSHTEIN_FAST  ----- */
}

inline size_t
sw                 (const char     *seq1,
                    const char     *seq2,
                    penalty_mat_t   pnlty_mat)
{
    size_t      seq1_l;
    size_t      seq2_l;
    int        *score_matrix;

    seq1_l          = strlen(seq1);
    seq2_l          = strlen(seq2);
    score_matrix    = calloc((seq1_l + 1) * (seq2_l + 1), sizeof(int));





