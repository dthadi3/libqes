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
    return seq;
}

seq_t *
create_seq_no_qual_or_comment ()
{
    seq_t *seq = km_malloc(sizeof(*seq));
    init_str(&seq->name, __INIT_LINE_LEN);
    init_str(&seq->seq, __INIT_LINE_LEN);
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

int
fill_name (seq_t *seqt, const char *name, size_t len)
{
    if (name != NULL) {
        str_fill_charptr_len(&seqt->name, name, len);
        return 1;
    } return 0;
}

int
fill_comment (seq_t *seqt, const char *comment, size_t len)
{
    if (comment != NULL) {
        str_fill_charptr_len(&seqt->comment, comment, len);
        return 1;
    } return 0;
}

int
fill_seq (seq_t *seqt, const char *seq, size_t len)
{
    if (seq != NULL) {
        str_fill_charptr_len(&seqt->seq, seq, len);
        return 1;
    } return 0;
}

int
fill_qual (seq_t *seqt, const char *qual, size_t len)
{
    if (qual != NULL) {
        str_fill_charptr_len(&seqt->qual, qual, len);
        return 1;
    } return 0;
}

int
fill_header (seq_t *seqt, const char *header, size_t len)
{
    if (header != NULL) {
        char *tmp = strchr(header, ' ');
        if (tmp != NULL) {
            tmp[0] = '\0';
            str_fill_charptr_len(&seqt->name, header, tmp - header);
            str_fill_charptr(&seqt->comment, tmp + 1);
        } else {
            str_fill_charptr(&seqt->name, header);
            str_nullify(&seqt->name);
        }
    }
    return 1;
}

void
destroy_seq_ (seq_t *seq)
{
    destroy_str_cp(&seq->name);
    destroy_str_cp(&seq->comment);
    destroy_str_cp(&seq->seq);
    destroy_str_cp(&seq->qual);
    km_free(seq);
}
