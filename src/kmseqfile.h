/*
 * ============================================================================
 *
 *       Filename:  kmseqfile.h
 *
 *    Description:  kmseqfile -- read sequences in FASTA or FASTQ format.
 *
 *        Version:  1.0
 *        Created:  14/05/14 15:57:20
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#ifndef KMSEQFILE_H
#define KMSEQFILE_H

#include <kmutil.h>
#include <kmseq.h>
#include <kmzfile.h>

/*---------------------------------------------------------------------------
  |  Seqfile methods -- seamless reading & writing of FASTA & FASTQ         |
  ---------------------------------------------------------------------------*/

#define    FASTA_DELIM '>'
#define    FASTQ_DELIM '@'
#define    FASTQ_QUAL_DELIM '+'

typedef enum _seqfile_format {
    UNKNOWN_FMT = 0,
    FASTA_FMT = 1,
    FASTQ_FMT = 2,
} seqfile_format_t;

typedef struct __seqfile_flags {
    unsigned int format     :2;
} seqfile_flags_t;

typedef struct _kmseqfile {
    zfile_t *zf;
    seqfile_flags_t flags;
    size_t n_records;
} seqfile_t;



/*===  FUNCTION  ============================================================*
Name:           seqfile_create
Paramters:      const char *path: Path to open.
                const char *mode: Mode to pass to the fopen equivalent used.
Description:    Allocates structures, initialises values and opens the internal
                file handle.
Returns:        A fully usable ``seqfile_t *`` or NULL.
 *===========================================================================*/
seqfile_t *seqfile_create (const char *path, const char *mode);


/*===  FUNCTION  ============================================================*
Name:           seqfile_ok
Paramters:      const seqfile_t *file: File reference to check.
Description:    Check a file referenece ``file``, ensuring that it may be
                dereferenced and used. This checks if a buffer has been
                allocated and buffer pointers set appropriately IFF the file is
                opened for reading. No guarantees are given about being able to
                read from the file.
Returns:        An int evaluating to true if ``file`` is OK, otherwise false.
 *===========================================================================*/
extern int seqfile_ok(const seqfile_t *file);


/*===  FUNCTION  ============================================================*
Name:           seqfile_guess_format
Paramters:      seqfile_t *file: File whose format we guess.
Description:    Guess, using zfpeek, the format of ``file``. This only inspects
                the first character, so it will be confused if the first
                character of the file is incorrect. Only use this at the start
                of a file (this is checked).
Returns:        The file's format as a seqfile_format_t, or UNKNOWN_FMT if the
                file is not readable, or not a FASTA/FASTQ file.
 *===========================================================================*/
seqfile_format_t seqfile_guess_format(seqfile_t *file);

void seqfile_set_format (seqfile_t *file, seqfile_format_t format);

extern ssize_t seqfile_read (seqfile_t *file, seq_t *seq);

extern ssize_t seqfile_write (seqfile_t *file, seq_t *seq);

extern size_t seqfile_format_seq(const seq_t *seq, seqfile_format_t fmt,
        char *buffer, size_t maxlen);

void seqfile_destroy_(seqfile_t *seqfile);
#define seqfile_destroy(seqfile) do {                                       \
            seqfile_destroy_(seqfile);                                      \
            seqfile = NULL;                                                 \
        } while(0)

#define SEQFILE_ITER_PARALLEL_SINGLE_BEGIN(fle, sq, ln, opts)               \
    _Pragma(STRINGIFY(omp parallel shared(fle) opts default(none)))         \
    {                                                                       \
        seq_t *sq = seq_create();                                           \
        ssize_t ln = 0;                                                     \
        while(1) {                                                          \
            _Pragma(STRINGIFY(omp critical))                                \
            {                                                               \
                ln = seqfile_read(fle, sq);                                 \
            }                                                               \
            if (ln < 0) {                                                   \
                break;                                                      \
            }

#define SEQFILE_ITER_PARALLEL_SINGLE_END(sq)                                \
        }                                                                   \
        seq_destroy(sq);                                                    \
    }

#define SEQFILE_ITER_PARALLEL_PAIRED_BEGIN(fle1, fle2, sq1, sq2, ln1, ln2, opts)\
    _Pragma(STRINGIFY(omp parallel shared(fle1, fle2) opts default(none)))  \
    {                                                                       \
        seq_t *sq1 = seq_create();                                          \
        seq_t *sq2 = seq_create();                                          \
        ssize_t ln1 = 0;                                                    \
        ssize_t ln2 = 0;                                                    \
        while(1) {                                                          \
            _Pragma(STRINGIFY(omp critical))                                \
            {                                                               \
                ln1 = seqfile_read(fle1, sq1);                              \
                ln2 = seqfile_read(fle2, sq2);                              \
            }                                                               \
            if (ln1 < 0 || ln2 < 0) {                                       \
                break;                                                      \
            }

#define SEQFILE_ITER_PARALLEL_PAIRED_END(sq1, sq2)                          \
        }                                                                   \
        seq_destroy(sq1);                                                   \
        seq_destroy(sq2);                                                   \
    }

#define SEQFILE_ITER_PARALLEL_INTERLEAVED_BEGIN(fle, sq1, sq2, ln1, ln2, opts)\
    _Pragma(STRINGIFY(omp parallel shared(fle) opts default(none)))         \
    {                                                                       \
        seq_t *sq1 = seq_create();                                          \
        seq_t *sq2 = seq_create();                                          \
        ssize_t ln1 = 0;                                                    \
        ssize_t ln2 = 0;                                                    \
        while(1) {                                                          \
            _Pragma(STRINGIFY(omp critical))                                \
            {                                                               \
                ln1 = seqfile_read(fle, sq1);                               \
                ln2 = seqfile_read(fle, sq2);                               \
            }                                                               \
            if (ln1 < 0 || ln2 < 0) {                                       \
                break;                                                      \
            }

#define SEQFILE_ITER_PARALLEL_INTERLEAVED_END(sq1, sq2)                     \
        }                                                                   \
        seq_destroy(sq1);                                                   \
        seq_destroy(sq2);                                                   \
    }

#define SEQFILE_ITER_SINGLE_BEGIN(fle, sq, ln)                              \
    {                                                                       \
        seq_t *sq = seq_create();                                           \
        ssize_t ln = 0;                                                     \
        while(1) {                                                          \
            ln = seqfile_read(fle, sq);                                     \
            if (ln < 0) {                                                   \
                break;                                                      \
            }

#define SEQFILE_ITER_SINGLE_END(sq)                                         \
        }                                                                   \
        seq_destroy(sq);                                                    \
    }

#define SEQFILE_ITER_PAIRED_BEGIN(fle1, fle2, sq1, sq2, ln1, ln2)           \
    {                                                                       \
        seq_t *sq1 = seq_create();                                          \
        seq_t *sq2 = seq_create();                                          \
        ssize_t ln1 = 0;                                                    \
        ssize_t ln2 = 0;                                                    \
        while(1) {                                                          \
            ln1 = seqfile_read(fle1, sq1);                                  \
            ln2 = seqfile_read(fle2, sq2);                                  \
            if (ln1 < 0 || ln2 < 0) {                                       \
                break;                                                      \
            }

#define SEQFILE_ITER_PAIRED_END(sq1, sq2)                                   \
        }                                                                   \
        seq_destroy(sq1);                                                   \
        seq_destroy(sq2);                                                   \
    }

#define SEQFILE_ITER_INTERLEAVED_BEGIN(fle, sq1, sq2, ln1, ln2)             \
    {                                                                       \
        seq_t *sq1 = seq_create();                                          \
        seq_t *sq2 = seq_create();                                          \
        ssize_t ln1 = 0;                                                    \
        ssize_t ln2 = 0;                                                    \
        while(1) {                                                          \
            ln1 = seqfile_read(fle, sq1);                                   \
            ln2 = seqfile_read(fle, sq2);                                   \
            if (ln1 < 0 || ln2 < 0) {                                       \
                break;                                                      \
            }

#define SEQFILE_ITER_INTERLEAVED_END(sq1, sq2)                              \
        }                                                                   \
        seq_destroy(sq1);                                                   \
        seq_destroy(sq2);                                                   \
    }

#endif /* KMSEQFILE_H */
