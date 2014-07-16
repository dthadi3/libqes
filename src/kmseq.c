/*
 * ============================================================================
 *
 *       Filename:  kmseq.c
 *
 *    Description:  A kseq.h-like buffered sequence file parser
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */

#include "kmseq.h"

seq_t *
seq_create (void)
{
    seq_t *seq = km_malloc(sizeof(*seq));

    init_str(&seq->name, __INIT_LINE_LEN);
    init_str(&seq->comment, __INIT_LINE_LEN);
    init_str(&seq->seq, __INIT_LINE_LEN);
    init_str(&seq->qual, __INIT_LINE_LEN);
    return seq;
}

seq_t *
seq_create_no_qual (void)
{
    seq_t *seq = km_malloc(sizeof(*seq));

    init_str(&seq->name, __INIT_LINE_LEN);
    init_str(&seq->comment, __INIT_LINE_LEN);
    init_str(&seq->seq, __INIT_LINE_LEN);
    seq->qual.m = 0;
    seq->qual.l = 0;
    seq->qual.s = NULL;
    return seq;
}

seq_t *
seq_create_no_qual_or_comment (void)
{
    seq_t *seq = km_malloc(sizeof(*seq));
    init_str(&seq->name, __INIT_LINE_LEN);
    init_str(&seq->seq, __INIT_LINE_LEN);
    seq->qual.m = 0;
    seq->qual.l = 0;
    seq->qual.s = NULL;
    seq->comment.m = 0;
    seq->comment.l = 0;
    seq->comment.s = NULL;
    return seq;
}

inline int
seq_ok (const seq_t *seq)
{
    return \
       seq != NULL && \
       str_ok(&seq->name) && \
       str_ok(&seq->comment) && \
       str_ok(&seq->seq) && \
       str_ok(&seq->qual);
}

inline int
seq_ok_no_comment (const seq_t *seq)
{
    return \
       seq != NULL && \
       str_ok(&seq->name) && \
       str_ok(&seq->seq) && \
       str_ok(&seq->qual);
}

inline int
seq_ok_no_qual (const seq_t *seq)
{
    return \
        seq != NULL && \
        str_ok(&seq->name) && \
        str_ok(&seq->comment) && \
        str_ok(&seq->seq);
}

inline int
seq_ok_no_comment_or_qual (const seq_t *seq)
{
    return \
        seq != NULL && \
        str_ok(&seq->name) && \
        str_ok(&seq->seq);
}

inline int
seq_fill_name (seq_t *seqobj, const char *name, size_t len)
{
    if (seqobj == NULL || name == NULL || len < 1) {
        return 0;
    }
    str_fill_charptr(&seqobj->name, name, len);
    return 1;
}

inline int
seq_fill_comment (seq_t *seqobj, const char *comment, size_t len)
{
    if (seqobj == NULL || comment == NULL || len < 1) {
        return 0;
    }
    str_fill_charptr(&seqobj->comment, comment, len);
    return 1;
}

inline int
seq_fill_seq (seq_t *seqobj, const char *seq, size_t len)
{
    if (seqobj == NULL || seq == NULL || len < 1) {
        return 0;
    }
    str_fill_charptr(&seqobj->seq, seq, len);
    return 1;
}

inline int
seq_fill_qual (seq_t *seqobj, const char *qual, size_t len)
{
    if (seqobj == NULL || qual == NULL || len < 1) {
        return 0;
    }
    str_fill_charptr(&seqobj->qual, qual, len);
    return 1;
}

inline int
seq_fill_header (seq_t *seqobj, char *header, size_t len)
{
    char *tmp = NULL;
    size_t startfrom = 0;

    if (seqobj == NULL || header == NULL || len < 1) {
        return 0;
    }
    while (isspace(header[len-1])) {
        header[--len] = '\0';
    }
    tmp = memchr(header, ' ', len);
    startfrom = header[0] == '@' || header[0] == '>' ? 1 : 0;
    if (tmp != NULL) {
        str_fill_charptr(&seqobj->name, header + startfrom,
                tmp - header - startfrom);
        str_fill_charptr(&seqobj->comment, tmp + 1, 0);
    } else {
        str_fill_charptr(&seqobj->name, header + startfrom, len - startfrom);
        str_nullify(&seqobj->comment);
    }
    return 1;
}

/*===  FUNCTION  ============================================================*
Name:           seq_destroy
Paramters:      seq_t *: seq to destroy.
Description:    Deallocate and set to NULL a seq_t on the heap.
Returns:        void.
 *===========================================================================*/
void
seq_destroy_ (seq_t *seq)
{
    if (seq != NULL) {
        destroy_str_cp(&seq->name);
        destroy_str_cp(&seq->comment);
        destroy_str_cp(&seq->seq);
        destroy_str_cp(&seq->qual);
        km_free(seq);
    }
}
