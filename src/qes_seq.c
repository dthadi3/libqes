/*
 * ============================================================================
 *
 *       Filename:  qes_seq.c
 *
 *    Description:  Sequence structures
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_seq.h"

struct qes_seq *
qes_seq_create (void)
{
    struct qes_seq *seq = qes_malloc(sizeof(*seq));

    qes_str_init(&seq->name, __INIT_LINE_LEN);
    qes_str_init(&seq->comment, __INIT_LINE_LEN);
    qes_str_init(&seq->seq, __INIT_LINE_LEN);
    qes_str_init(&seq->qual, __INIT_LINE_LEN);
    return seq;
}

struct qes_seq *
qes_seq_create_no_qual (void)
{
    struct qes_seq *seq = qes_malloc(sizeof(*seq));

    qes_str_init(&seq->name, __INIT_LINE_LEN);
    qes_str_init(&seq->comment, __INIT_LINE_LEN);
    qes_str_init(&seq->seq, __INIT_LINE_LEN);
    seq->qual.m = 0;
    seq->qual.l = 0;
    seq->qual.s = NULL;
    return seq;
}

struct qes_seq *
qes_seq_create_no_qual_or_comment (void)
{
    struct qes_seq *seq = qes_malloc(sizeof(*seq));
    qes_str_init(&seq->name, __INIT_LINE_LEN);
    qes_str_init(&seq->seq, __INIT_LINE_LEN);
    seq->qual.m = 0;
    seq->qual.l = 0;
    seq->qual.s = NULL;
    seq->comment.m = 0;
    seq->comment.l = 0;
    seq->comment.s = NULL;
    return seq;
}


inline int
qes_seq_fill_name (struct qes_seq *seqobj, const char *name, size_t len)
{
    if (seqobj == NULL || name == NULL || len < 1) {
        return 0;
    }
    qes_str_fill_charptr(&seqobj->name, name, len);
    return 1;
}

inline int
qes_seq_fill_comment (struct qes_seq *seqobj, const char *comment, size_t len)
{
    if (seqobj == NULL || comment == NULL || len < 1) {
        return 0;
    }
    qes_str_fill_charptr(&seqobj->comment, comment, len);
    return 1;
}

inline int
qes_seq_fill_seq (struct qes_seq *seqobj, const char *seq, size_t len)
{
    if (seqobj == NULL || seq == NULL || len < 1) {
        return 0;
    }
    qes_str_fill_charptr(&seqobj->seq, seq, len);
    return 1;
}

inline int
qes_seq_fill_qual (struct qes_seq *seqobj, const char *qual, size_t len)
{
    if (seqobj == NULL || qual == NULL || len < 1) {
        return 0;
    }
    qes_str_fill_charptr(&seqobj->qual, qual, len);
    return 1;
}

inline int
qes_seq_fill_header (struct qes_seq *seqobj, char *header, size_t len)
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
        qes_str_fill_charptr(&seqobj->name, header + startfrom,
                tmp - header - startfrom);
        qes_str_fill_charptr(&seqobj->comment, tmp + 1, 0);
    } else {
        qes_str_fill_charptr(&seqobj->name, header + startfrom, len - startfrom);
        qes_str_nullify(&seqobj->comment);
    }
    return 1;
}

inline int
qes_seq_fill(struct qes_seq *seqobj, const char *name, const char *comment,
             const char *seq, const char *qual)
{
    if (!qes_seq_ok(seqobj) || name == NULL || comment == NULL || seq == NULL \
            || qual == NULL) {
        return 1;
    }
    if (qes_seq_fill_name(seqobj, name, 0) != 0) return 1;
    if (qes_seq_fill_comment(seqobj, comment, 0) != 0) return 1;
    if (qes_seq_fill_seq(seqobj, seq, 0) != 0) return 1;
    if (qes_seq_fill_qual(seqobj, qual, 0) != 0) return 1;
    return 0;
}

/*===  FUNCTION  ============================================================*
Name:           qes_seq_destroy
Paramters:      struct qes_seq *: seq to destroy.
Description:    Deallocate and set to NULL a struct qes_seq on the heap.
Returns:        void.
 *===========================================================================*/
void
qes_seq_destroy_ (struct qes_seq *seq)
{
    if (seq != NULL) {
        qes_str_destroy_cp(&seq->name);
        qes_str_destroy_cp(&seq->comment);
        qes_str_destroy_cp(&seq->seq);
        qes_str_destroy_cp(&seq->qual);
        qes_free(seq);
    }
}
