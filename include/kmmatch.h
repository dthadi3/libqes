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

#include <string.h>
#include <stdint.h>

/*  Everyone loves a maze. A lovely macro maze! */
/*  Set tab to be 4 spaces or it will look like shit. Sorry Linus */

#define	__KM_CALCHAMMING_INIT(SCOPE)                                            \
SCOPE inline int calc_hamming(const char *seq1, const char *seq2) {             \
    int ham = 0;                                                                \
    int seqlen = 0;                                                             \
    int iii = 0;                                                                \
                                                                                \
    if (strlen(seq1) == strlen(seq2)) return -1;                                \
    seqlen = strlen(seq1);                                                      \
                                                                                \
    for (iii; iii < seqlen; iii++) {                                            \
        if (seq1[iii] != seq2[iii]) {                                           \
             ham++;                                                             \
        }                                                                       \
     }                                                                          \
    return ham;                                                                 \
}                                                                               \

/* 
#define	__KM_FINDMISMATCH_INIT(SCOPE)                                           \
SCOPE 

 */


#define	KMMATCH_INIT(SCOPE)			                                            \
    __KM_CALCHAMMING_INIT(SCOPE)                                                \


#endif /* KMMATCH_H */
