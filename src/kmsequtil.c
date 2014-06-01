/*
 * ============================================================================
 *
 *       Filename:  kmsequtil.c
 *
 *    Description:  Sequence utility functions
 *
 *        Version:  1.0
 *        Created:  05/01/14 18:51:21
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */

#include "kmsequtil.h"

/*
 * ===  FUNCTION  =============================================================
 *         Name:  translate_codon
 *  Description:  translate a codon to an amino acid.
 * ============================================================================
 */

inline char
translate_codon (const char *codon)
{
    if (codon == NULL || strnlen(codon, 4) != 3) {
        return -1;
    }
    if (codon[0] == 'A') {
        if (codon[1] == 'A') {
            if (codon[2] == 'A') return 'K';
            else if (codon[2] == 'C') return 'N';
            else if (codon[2] == 'G') return 'K';
            else if (codon[2] == 'T') return 'N';
            else if (codon[2] == 'U') return 'N';
         }
        else if (codon[1] == 'C') {
            if (codon[2] == 'A') return 'T';
            else if (codon[2] == 'C') return 'T';
            else if (codon[2] == 'G') return 'T';
            else if (codon[2] == 'T') return 'T';
            else if (codon[2] == 'U') return 'T';
         }
        else if (codon[1] == 'G') {
            if (codon[2] == 'A') return 'R';
            else if (codon[2] == 'C') return 'S';
            else if (codon[2] == 'G') return 'R';
            else if (codon[2] == 'T') return 'S';
            else if (codon[2] == 'U') return 'S';
         }
        else if (codon[1] == 'T') {
            if (codon[2] == 'A') return 'I';
            else if (codon[2] == 'C') return 'I';
            else if (codon[2] == 'G') return 'M';
            else if (codon[2] == 'T') return 'I';
            else if (codon[2] == 'U') return 'I';
         }
        else if (codon[1] == 'U') {
            if (codon[2] == 'A') return 'I';
            else if (codon[2] == 'C') return 'I';
            else if (codon[2] == 'G') return 'M';
            else if (codon[2] == 'T') return 'I';
            else if (codon[2] == 'U') return 'I';
         }
    }
    else if (codon[0] == 'C') {
        if (codon[1] == 'A') {
            if (codon[2] == 'A') return 'Q';
            else if (codon[2] == 'C') return 'H';
            else if (codon[2] == 'G') return 'Q';
            else if (codon[2] == 'T') return 'H';
            else if (codon[2] == 'U') return 'H';
         }
        else if (codon[1] == 'C') {
            if (codon[2] == 'A') return 'P';
            else if (codon[2] == 'C') return 'P';
            else if (codon[2] == 'G') return 'P';
            else if (codon[2] == 'T') return 'P';
            else if (codon[2] == 'U') return 'P';
         }
        else if (codon[1] == 'G') {
            if (codon[2] == 'A') return 'R';
            else if (codon[2] == 'C') return 'R';
            else if (codon[2] == 'G') return 'R';
            else if (codon[2] == 'T') return 'R';
            else if (codon[2] == 'U') return 'R';
         }
        else if (codon[1] == 'T') {
            if (codon[2] == 'A') return 'L';
            else if (codon[2] == 'C') return 'L';
            else if (codon[2] == 'G') return 'L';
            else if (codon[2] == 'T') return 'L';
            else if (codon[2] == 'U') return 'L';
         }
        else if (codon[1] == 'U') {
            if (codon[2] == 'A') return 'L';
            else if (codon[2] == 'C') return 'L';
            else if (codon[2] == 'G') return 'L';
            else if (codon[2] == 'T') return 'L';
            else if (codon[2] == 'U') return 'L';
         }
    }
    else if (codon[0] == 'G') {
        if (codon[1] == 'A') {
            if (codon[2] == 'A') return 'E';
            else if (codon[2] == 'C') return 'D';
            else if (codon[2] == 'G') return 'E';
            else if (codon[2] == 'T') return 'D';
            else if (codon[2] == 'U') return 'D';
         }
        else if (codon[1] == 'C') {
            if (codon[2] == 'A') return 'A';
            else if (codon[2] == 'C') return 'A';
            else if (codon[2] == 'G') return 'A';
            else if (codon[2] == 'T') return 'A';
            else if (codon[2] == 'U') return 'A';
         }
        else if (codon[1] == 'G') {
            if (codon[2] == 'A') return 'G';
            else if (codon[2] == 'C') return 'G';
            else if (codon[2] == 'G') return 'G';
            else if (codon[2] == 'T') return 'G';
            else if (codon[2] == 'U') return 'G';
         }
        else if (codon[1] == 'T') {
            if (codon[2] == 'A') return 'V';
            else if (codon[2] == 'C') return 'V';
            else if (codon[2] == 'G') return 'V';
            else if (codon[2] == 'T') return 'V';
            else if (codon[2] == 'U') return 'V';
         }
        else if (codon[1] == 'U') {
            if (codon[2] == 'A') return 'V';
            else if (codon[2] == 'C') return 'V';
            else if (codon[2] == 'G') return 'V';
            else if (codon[2] == 'T') return 'V';
            else if (codon[2] == 'U') return 'V';
         }
    }
    else if (codon[0] == 'T') {
        if (codon[1] == 'A') {
            if (codon[2] == 'A') return '*';
            else if (codon[2] == 'C') return 'Y';
            else if (codon[2] == 'G') return '*';
            else if (codon[2] == 'T') return 'Y';
            else if (codon[2] == 'U') return 'Y';
         }
        else if (codon[1] == 'C') {
            if (codon[2] == 'A') return 'S';
            else if (codon[2] == 'C') return 'S';
            else if (codon[2] == 'G') return 'S';
            else if (codon[2] == 'T') return 'S';
            else if (codon[2] == 'U') return 'S';
         }
        else if (codon[1] == 'G') {
            if (codon[2] == 'A') return '*';
            else if (codon[2] == 'C') return 'C';
            else if (codon[2] == 'G') return 'W';
            else if (codon[2] == 'T') return 'C';
            else if (codon[2] == 'U') return 'C';
         }
        else if (codon[1] == 'T') {
            if (codon[2] == 'A') return 'L';
            else if (codon[2] == 'C') return 'F';
            else if (codon[2] == 'G') return 'L';
            else if (codon[2] == 'T') return 'F';
            else if (codon[2] == 'U') return 'F';
         }
        else if (codon[1] == 'U') {
            if (codon[2] == 'A') return 'L';
            else if (codon[2] == 'C') return 'F';
            else if (codon[2] == 'G') return 'L';
            else if (codon[2] == 'T') return 'F';
            else if (codon[2] == 'U') return 'F';
         }
    }
    else if (codon[0] == 'U') {
        if (codon[1] == 'A') {
            if (codon[2] == 'A') return '*';
            else if (codon[2] == 'C') return 'Y';
            else if (codon[2] == 'G') return '*';
            else if (codon[2] == 'T') return 'Y';
            else if (codon[2] == 'U') return 'Y';
         }
        else if (codon[1] == 'C') {
            if (codon[2] == 'A') return 'S';
            else if (codon[2] == 'C') return 'S';
            else if (codon[2] == 'G') return 'S';
            else if (codon[2] == 'T') return 'S';
            else if (codon[2] == 'U') return 'S';
         }
        else if (codon[1] == 'G') {
            if (codon[2] == 'A') return '*';
            else if (codon[2] == 'C') return 'C';
            else if (codon[2] == 'G') return 'W';
            else if (codon[2] == 'T') return 'C';
            else if (codon[2] == 'U') return 'C';
         }
        else if (codon[1] == 'T') {
            if (codon[2] == 'A') return 'L';
            else if (codon[2] == 'C') return 'F';
            else if (codon[2] == 'G') return 'L';
            else if (codon[2] == 'T') return 'F';
            else if (codon[2] == 'U') return 'F';
         }
        else if (codon[1] == 'U') {
            if (codon[2] == 'A') return 'L';
            else if (codon[2] == 'C') return 'F';
            else if (codon[2] == 'G') return 'L';
            else if (codon[2] == 'T') return 'F';
            else if (codon[2] == 'U') return 'F';
         }
    }
    return 'X';
}


inline char *
revcomp (const char *seq, size_t len)
{
	int seqlen = strlen(seq);
    seqlen = seqlen < len ? seqlen : len - 1;
    char *outseq = strdup(seq);

	if (outseq[seqlen - 1] == '\n') {
		outseq[seqlen - 1] = '\0';
		seqlen--;
	}

    revcomp_inplace(outseq, len);
    return outseq;
}

inline void
revcomp_inplace (char *seq, size_t len)
{
	int seqlen = strlen(seq);
	int iii;
    seqlen = seqlen < len ? seqlen : len - 1;

	if (seq[seqlen - 1] == '\n') {
		seq[seqlen - 1] = '\0';
		seqlen--;
	}

	for (iii = 0; iii < seqlen; iii++) {
		int endpos = seqlen - iii - 1;
        char endchar = seq[endpos];
		if (seq[iii] == 'a' || seq[iii] == 'A') seq[endpos] = 'T';
		if (seq[iii] == 'c' || seq[iii] == 'C') seq[endpos] = 'G';
		if (seq[iii] == 'g' || seq[iii] == 'G') seq[endpos] = 'C';
		if (seq[iii] == 't' || seq[iii] == 'T') seq[endpos] = 'A';

		if (endchar == 'a' || endchar == 'A') seq[iii] = 'T';
		if (endchar == 'c' || endchar == 'C') seq[iii] = 'G';
		if (endchar == 'g' || endchar == 'G') seq[iii] = 'C';
		if (endchar == 't' || endchar == 'T') seq[iii] = 'A';
	}
}
