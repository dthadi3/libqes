/*
 * ============================================================================
 *
 *       Filename:  kmutil.h
 *
 *    Description:  Utility functions of kmlib
 *
 *        Version:  1.0
 *        Created:  05/01/14 18:51:21
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */
#ifndef KMUTIL_H
#define KMUTIL_H

#include "kmbase.h"

extern char translate_codon(char *codon);
extern char *revcomp(const char *seq, size_t len);
extern void revcomp_inplace(char *seq, size_t len);

#endif /* KMUTIL_H */
