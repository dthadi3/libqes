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

#define	FASTA_DELIM '>'
#define	FASTQ_DELIM '@'
#define	FASTQ_QUAL_DELIM '+'

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
Name:           create_seqfile
Paramters:      const char *path: Path to open.
                const char *mode: Mode to pass to the fopen equivalent used.
Description:    Allocates structures, initialises values and opens the internal
                file handle.
Returns:        A fully usable ``seqfile_t *`` or NULL.
 *===========================================================================*/
seqfile_t *create_seqfile (const char *path, const char *mode);


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

extern ssize_t read_seqfile (seqfile_t *file, seq_t *seq);

extern ssize_t write_seqfile (seqfile_t *file, seq_t *seq);

extern size_t strfseq(const seq_t *seq, seqfile_format_t fmt, char *buffer,
        size_t maxlen);
void print_seq_seqfile (seqfile_t * file, const seq_t *seq);

void destroy_seqfile_(seqfile_t *seqfile);
#define destroy_seqfile(seqfile) do {       \
            destroy_seqfile_(seqfile);      \
            seqfile = NULL;             \
        } while(0)

#if 0
/*
 * Collection of options to a seqfile_iter function.
 */
typedef struct __seqfile_iter_flags {
    int die_quickly         :1;
    int                     :7; /* Fill to byte */
    int num_threads         :8; /* Don't know of anything w/ > 255 threads */
} seqfile_iter_flags;

/* Function pointer that takes a seq_t and a pointer to arbitrary data and
   returns a success/failure value */
typedef int (*seqfile_iter_func_t)(const seq_t*, void *);

int seqfile_iter_parallel (seqfile_t *file, seqfile_iter_func_t func,
        void *data, seqfile_iter_flags flags);
int seqfile_iter (seqfile_t *file, seqfile_iter_func_t func, void *data,
        seqfile_iter_flags flags);
#endif

#endif /* KMSEQFILE_H */
