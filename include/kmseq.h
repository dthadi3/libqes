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
#define KMSEQ_STR_LEN (size_t)(1<<14)       /* 16384 */
#define KMSEQ_SEQ_LEN (size_t)(1<<8)        /* 256 */

#define	KMSEQ_QUAL

typedef struct {
    char *name;
    char *seq;
    char *qual;
    size_t seqlen;
#ifdef  KMSEQ_QUAL
} kmseq;
#else      /* -----  not KMSEQ_QUAL  ----- */
} kmseq_q;
#endif     /* -----  not KMSEQ_QUAL  ----- */


typedef struct {
    char *name;
    char *seq;
    size_t seqlen;
#ifdef  KMSEQ_QUAL
} kmseq_noq;
#else      /* -----  not KMSEQ_QUAL  ----- */
} kmseq;
#endif     /* -----  not KMSEQ_QUAL  ----- */



kmseq *create_kmseq();
int fill_kmseq(kmseq *seqref, const char *name, const char *seq,
        const char *qual);
void destroy_kmseq(kmseq *seq);
kmseq *read_seq_file(kmfile *file);
void print_kmseq (kmseq const *seq, FILE *stream);


#endif /* KMSEQ_H */
