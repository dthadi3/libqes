/*
 * ============================================================================
 *
 *       Filename:  kmsequtil.h
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
 *
 * ============================================================================
 */
#ifndef KMSEQUTIL_H
#define KMSEQUTIL_H

#include <kmutil.h>

extern char translate_codon(const char *codon);
extern char *revcomp(const char *seq, size_t len);
extern void revcomp_inplace(char *seq, size_t len);

#endif /* KMSEQUTIL_H */
