/*
 * ============================================================================
 *
 *       Filename:  qes_seq.h
 *
 *    Description:  Sequence structures
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

#ifndef QES_SEQ_H
#define QES_SEQ_H

#include <qes_util.h>
#include <qes_str.h>
#include <ctype.h>


/*---------------------------------------------------------------------------
  | qes_seq module -- data structures to hold NGS sequences                   |
  ---------------------------------------------------------------------------*/

/* TYPE DEFINITIONS */
typedef struct _seq {
    struct qes_str name;
    struct qes_str comment;
    struct qes_str seq;
    struct qes_str qual;
} seq_t;

/* PROTOTYPES */

/*===  FUNCTION  ============================================================*
Name:           seq_create
Paramters:      void
Description:    Create a ``seq_t`` object on the heap, creating and
                initialising all members. seq_create_no_X functions do not
                create or initialise the members in their names.
Returns:        seq_t *: A non-null memory address on success, otherwise NULL.
 *===========================================================================*/
seq_t *seq_create (void);
seq_t *seq_create_no_qual (void);
seq_t *seq_create_no_qual_or_comment (void);


/*===  FUNCTION  ============================================================*
Name:           seq_ok
Paramters:      seq_t *: seq to check
Description:    Check if ``seq`` is a usable seq_t struct. seq_ok_no-X
                functions permit the member(s) in their names to be unusable.
Returns:        1 if usable, 0 otherwise.
 *===========================================================================*/
static inline int
seq_ok (const seq_t *seq)
{
    return \
       seq != NULL && \
       qes_str_ok(&seq->name) && \
       qes_str_ok(&seq->comment) && \
       qes_str_ok(&seq->seq) && \
       qes_str_ok(&seq->qual);
}

static inline int
seq_ok_no_comment (const seq_t *seq)
{
    return \
       seq != NULL && \
       qes_str_ok(&seq->name) && \
       qes_str_ok(&seq->seq) && \
       qes_str_ok(&seq->qual);
}

static inline int
seq_ok_no_qual (const seq_t *seq)
{
    return \
        seq != NULL && \
        qes_str_ok(&seq->name) && \
        qes_str_ok(&seq->comment) && \
        qes_str_ok(&seq->seq);
}

static inline int
seq_ok_no_comment_or_qual (const seq_t *seq)
{
    return \
        seq != NULL && \
        qes_str_ok(&seq->name) && \
        qes_str_ok(&seq->seq);
}

static inline int
seq_has_comment (const seq_t *seq)
{
    return seq_ok(seq) && seq->comment.l > 0;
}

static inline int
seq_has_qual (const seq_t *seq)
{
    return seq_ok(seq) && seq->qual.l > 0;
}

static inline int
seq_n_bytes (const seq_t *seq)
{
    if (!seq_ok(seq)) {
        return -1;
    }
    /* Arragned per line in a fastq */
    return 1 + seq->seq.l + seq_has_comment(seq) * 1 + seq->comment.l + 1 + \
           seq->seq.l + 1 +\
           2 * seq_has_qual(seq) + \
           seq->qual.l + seq_has_qual(seq) * 1;
}

/*===  FUNCTION  ============================================================*
Name:           seq_fill_header
Paramters:      seq_t *seqobj: Seq object that will receive the header.
Description:    Fills the name and comment members of a ``seq_t`` from the
                header line of a fasta/fastq file.
Returns:        int: 1 on success, otherwise 0 for failure.
 *===========================================================================*/
extern int seq_fill_header(seq_t *seqobj, char *header, size_t len);


/*===  FUNCTION  ============================================================*
Name:           seq_fill_X
Paramters:      These functions take a ``seq_t``, a char array and the length
                of the char array as a size_t.
Description:    Fill a seq_t's name, comment, seq or qual member from a char
                array. If a non-zero value is given to ``len``, it is assumed
                to be the length of the string, otherwise the length of the
                string is calculated using strlen.
Returns:        int: 1 on success, 0 on failure.
 *===========================================================================*/
extern int seq_fill_seq(seq_t *seqobj, const char *seq, size_t len);
extern int seq_fill_qual(seq_t *seqobj, const char *qual, size_t len);
extern int seq_fill_name(seq_t *seqobj, const char *name, size_t len);
extern int seq_fill_comment(seq_t *seqobj, const char *comment, size_t len);


/*===  FUNCTION  ============================================================*
Name:           seq_print
Paramters:      const seq_t *: seq to print
                qes_seqfile_format_t: file format to print in.
                FILE *: open file stream to print to.
Description:    Print ``seq`` in formatted per ``format`` to ``stream``.
Returns:        int: 1 on success, 0 on failure.
 *===========================================================================*/
extern int seq_print (const seq_t *seq, FILE *stream);

/*===  FUNCTION  ============================================================*
Name:           seq_destroy
Paramters:      seq_t *: seq to destroy.
Description:    Deallocate and set to NULL a seq_t on the heap.
Returns:        void.
 *===========================================================================*/
void seq_destroy_(seq_t *seq);
#define seq_destroy(seq) do {       \
            seq_destroy_(seq);      \
            seq = NULL;             \
        } while(0)


#endif /* QES_SEQ_H */
