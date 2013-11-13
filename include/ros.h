/*
 * =====================================================================================
 *
 *       Filename:  kmlib.h
 *
 *    Description:  A series of functions used in bioinformatic tasks
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray
 *
 * =====================================================================================
 */
#ifndef KMLIB_H
#define KMLIB_H

#include <stdlib.h>
#include <string.h>

#include "kseq.h"

int calc_hamming(const char *seq1, const char *seq2);
inline void revcomp(char **outseq, char *seq);

#endif /* KMLIB_H */
