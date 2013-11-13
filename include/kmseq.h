/*
 * =====================================================================================
 *
 *       Filename:  kmseq.h
 *
 *    Description:  A kseq.h-like buffered sequence file parser
 *
 *        Version:  0.0
 *        Created:  31/07/13 12:51:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray
 *
 * =====================================================================================
 */

#ifndef KMSEQ_H
#define KMSEQ_H

/* MACROS */
#define ALLOC_ERROR fprintf(stderr, "Failed to allocate memory\n"); exit(EXIT_FAILURE);
#define CHECK_ALLOC(pointer) if(!pointer) { ALLOC_ERROR }
#define FREE(pointer) free(pointer); pointer = NULL;

/*  CONSTANTS  */
#define KMFILE_BUFFER_LEN (size_t)(1<<20)   /* 1Mb */
#define KMSEQ_HEADER_LEN (size_t)(1<<8)     /* 256 */
#define KMSEQ_SEQ_LEN (size_t)(1<<8)        /* 256 */

#include <kmbase.h>

#include <zlib.h>

typedef struct {
    gzFile file;
    char *path;
    int eof;
    int err;
    int filepos;
    /* buffer <= bufferiter < bufferend <= buffer + KMFILE_BUFFER_LEN - 1 */
    char *buffer;
    char *bufferiter;
    char *bufferend;
} kmfile;

typedef struct {
    char *name;
    char *seq;
} kmseq;

typedef struct {
    char *name;
    char *seq;
    char *qual_h;
    char *qual;
} kmseq_q;


kmfile *create_kmfile(const char * filename);
void destroy_kmfile(kmfile *file);
kmseq *read_seq_file(kmfile *file);

kmseq *init_kmseq();

void fill_kmseq(kmseq *seqref, const char *name, const char *seq, const char *qual_h, const char *qual);

void destroy_kmseq(kmseq *seq);


#endif /* KMSEQ_H */
