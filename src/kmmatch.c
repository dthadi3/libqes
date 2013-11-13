/*
 * =====================================================================================
 *
 *       Filename:  kmmatch.
 *
 *    Description:  Sequence matching and finding functions used in bioinformatic tasks
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au [include word penguin in subject]
 *
 * =====================================================================================
 */


inline void revcomp(char **outseq, char *seq) {
    int seqlen = strlen(seq);
    if (seq[seqlen - 1] == '\n') {
        seq[seqlen - 1] = '\0';
        seqlen = strlen(seq);
    }

    int iii = 0;
    for (iii; iii < seqlen; iii++) {
        int outpos = seqlen - iii - 1;
        if (seq[iii] == 'a' || seq[iii] == 'A') (*outseq)[outpos] = 'T';
        if (seq[iii] == 'c' || seq[iii] == 'C') (*outseq)[outpos] = 'G';
        if (seq[iii] == 'g' || seq[iii] == 'G') (*outseq)[outpos] = 'C';
        if (seq[iii] == 't' || seq[iii] == 'T') (*outseq)[outpos] = 'A';
        /*printf("seq at %i %c, out %i %c\n", iii, seq[iii], outpos, (*outseq)[outpos]);*/
    }
}
