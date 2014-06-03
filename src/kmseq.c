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
create_seq ()
{
    seq_t *seq = km_malloc(sizeof(*seq));

    init_str(&seq->name, __INIT_LINE_LEN);
    init_str(&seq->comment, __INIT_LINE_LEN);
    init_str(&seq->seq, __INIT_LINE_LEN);
    init_str(&seq->qual, __INIT_LINE_LEN);
    return seq;
}

seq_t *
create_seq_no_qual ()
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
create_seq_no_qual_or_comment ()
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

void
print_seq (const seq_t *seq, FILE *stream)
{
    fprintf(stream, "@");
    print_str(&seq->name, stream);
    fprintf(stream, " ");
    print_str(&seq->comment, stream);
    fprintf(stream, "\n");
    print_str(&seq->seq, stream);
    fprintf(stream, "\n+\n");
    print_str(&seq->qual, stream);
    fprintf(stream, "\n");
}

inline int
seq_fill_name (seq_t *seqobj, const char *name, size_t len)
{
    if (name != NULL) {
        str_fill_charptr(&seqobj->name, name, len);
        return 1;
    }
    return 0;
}

inline int
seq_fill_comment (seq_t *seqobj, const char *comment, size_t len)
{
    if (comment != NULL) {
        str_fill_charptr(&seqobj->comment, comment, len);
        return 1;
    }
    return 0;
}

inline int
seq_fill_seq (seq_t *seqobj, const char *seq, size_t len)
{
    if (seq != NULL) {
        str_fill_charptr(&seqobj->seq, seq, len);
        return 1;
    }
    return 0;
}

inline int
seq_fill_qual (seq_t *seqobj, const char *qual, size_t len)
{
    if (qual != NULL) {
        str_fill_charptr(&seqobj->qual, qual, len);
        return 1;
    }
    return 0;
}

inline int
seq_fill_header (seq_t *seqobj, const char *header, size_t len)
{
    if (header != NULL) {
        char *tmp = strchr(header, ' ');
        if (tmp != NULL) {
            tmp[0] = '\0';
            str_fill_charptr(&seqobj->name, header, tmp - header);
            str_fill_charptr(&seqobj->comment, tmp + 1, 0);
        } else {
            str_fill_charptr(&seqobj->name, header, len);
            str_nullify(&seqobj->name);
        }
    }
    return 1;
}

/*===  FUNCTION  ============================================================*
Name:           destroy_seq
Paramters:      seq_t *: seq to destroy.
Description:    Deallocate and set to NULL a seq_t on the heap.
Returns:        void.
 *===========================================================================*/
void
destroy_seq_ (seq_t *seq)
{
    if (seq != NULL) {
        destroy_str_cp(&seq->name);
        destroy_str_cp(&seq->comment);
        destroy_str_cp(&seq->seq);
        destroy_str_cp(&seq->qual);
        km_free(seq);
    }
}
