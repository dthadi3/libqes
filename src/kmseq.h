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


/*---------------------------------------------------------------------------
  | kmseq module -- data structures to hold NGS sequences                   |
  ---------------------------------------------------------------------------*/

/* TYPE DEFINITIONS */
typedef struct _seq {
    str_t name;
    str_t comment;
    str_t seq;
    str_t qual;
} seq_t;

/* PROTOTYPES */
seq_t *create_seq (void);
seq_t *create_seq_no_qual (void);
seq_t *create_seq_no_qual_or_comment (void);

extern int seq_ok(const seq_t *seq);
extern int seq_ok_no_qual(const seq_t *seq);
extern int seq_ok_no_comment(const seq_t *seq);
extern int seq_ok_no_comment_or_qual(const seq_t *seq);
extern int seq_fill_header(seq_t *seqobj, const char *header, size_t len);
extern int seq_fill_seq(seq_t *seqobj, const char *seq, size_t len);
extern int seq_fill_qual(seq_t *seqobj, const char *qual, size_t len);
extern int seq_fill_name(seq_t *seqobj, const char *name, size_t len);
extern int seq_fill_comment(seq_t *seqobj, const char *comment, size_t len);
void print_seq (const seq_t *seq, FILE *stream);

void destroy_seq_(seq_t *seq);
#define destroy_seq(seq) do {       \
            destroy_seq_(seq);      \
            seq = NULL;             \
        } while(0)




#endif /* KMSEQ_H */
