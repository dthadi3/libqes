/*
 * ============================================================================
 *
 *       Filename:  kmseq.h
 *
 *    Description:  A kseq.h-like buffered sequence file parser
 *
 *        Version:  1.0
 *        Created:  31/07/13 12:51:02
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef KMSEQ_H
#define KMSEQ_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <zlib.h>

#include <kmbase.h>
#include <kmfile.h>


/*  CONSTANTS  */
typedef struct _kmqseq {
    str_t *name;
    str_t *seq;
    str_t *qual;
} qseq_t;

typedef struct _kmseq {
    str_t *name;
    str_t *seq;
} seq_t;

typedef enum _seqfile_type {
    FASTA,
    FASTQ,
} seqfile_type_t;

typedef struct _kmseqfile {
    kmfile *file;
    seqfile_type_t type;
} seqfile_t;


kmseq *create_kmseq();
kmseqfile *create_kmseqfile();
int fill_kmseq(kmseq *seqref, const char *name, const char *seq,
        const char *qual);
void destroy_kmseq(kmseq *seq);
kmseq *read_seq_file(kmseqfile *file);
void print_kmseq (kmseq const *seq, FILE *stream);


#endif /* KMSEQ_H */
