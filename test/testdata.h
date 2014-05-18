/*
 * ============================================================================
 *
 *       Filename:  testdata.h
 *
 *    Description:  Data for tests of libkdm
 *
 *        Version:  1.0
 *        Created:  17/03/14 16:26:59
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef TESTDATA_H
#define TESTDATA_H

extern const size_t n_loremipsum_lines;
extern const size_t loremipsum_fsize;
extern const size_t loremipsum_line_lens[];
extern const char *loremipsum_lines[];
extern char *data_prefix;
extern char *out_prefix;
extern char *text_file;
extern char *gz_text_file;
extern char *fasta_file;
extern char *fastq_file;
extern char *gzfastq_file;
extern char *bzfastq_file;

#endif /* TESTDATA_H */
