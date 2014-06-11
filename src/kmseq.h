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
extern int seq_ok(const seq_t *seq);
extern int seq_ok_no_qual(const seq_t *seq);
extern int seq_ok_no_comment(const seq_t *seq);
extern int seq_ok_no_comment_or_qual(const seq_t *seq);

/*===  FUNCTION  ============================================================*
Name:           seq_fill_header
Paramters:      seq_t *seqobj: Seq object that will receive the header.
Description:    Fills the name and comment members of a ``seq_t`` from the
                header line of a fasta/fastq file.
Returns:        int: 1 on success, otherwise 0 for failure.
 *===========================================================================*/
extern int seq_fill_header(seq_t *seqobj, const char *header, size_t len);


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
                seqfile_format_t: file format to print in.
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


#endif /* KMSEQ_H */
