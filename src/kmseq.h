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
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef KMSEQ_H
#define KMSEQ_H

#include <kmutil.h>
#include <kmstr.h>
/*
 *  Seq methods -- data structures to hold NGS sequences
 */

/********************************** Types ************************************/


typedef struct _kmqseq {
    str_t *name;
    str_t *comment;
    str_t *seq;
    str_t *qual;
} seq_t;


seq_t *create_seq();
int fill_kmseq(kmseq *seqref, const char *header, const char *seq,
        const char *qual);
void destroy_kmseq(kmseq *seq);


/*---------------------------------------------------------------------------
  |  Seqfile methods -- seamless reading & writing of FASTA & FASTQ         |
  ---------------------------------------------------------------------------*/

typedef enum _seqfile_format {
    FASTA = 1,
    FASTQ = 2,
    UNKNOWN = 0,
} seqfile_format_t;

typedef struct __seqfile_flags {
    unsigned int format     :2;
    unsigned int writing    :1;
} seqfile_flags_t;

typedef struct _kmseqfile {
    zfile_t *zf;
    seqfile_type_t type;
    size_t n_records;
    seqfile_flags_t flags;
} seqfile_t;

/* Function pointer that takes a seq_t and a pointer to arbitrary data and
   returns a success/failure value */
typedef int (*seqfile_iter_func_t)(const seq_t*, void *);

/*
 * Collection of options to a seqfile_iter function.
 */
typedef struct __seqfile_iter_flags {
    int die_quickly         :1;
    int                     :7; /* Fill to byte */
    int num_threads         :8; /* Don't know of anything w/ > 255 threads */
} seqfile_iter_flags;

kmseqfile *create_seqfile(const char *path, const char *mode);
int seqfile_iter_parallel(seqfile_t *file, seqfile_iter_func_t func,
        void *data, seqfile_iter_flags flags);
int seqfile_iter(seqfile_t *file, seqfile_iter_func_t func, void *data,
        seqfile_iter_flags flags);

#endif /* KMSEQ_H */
