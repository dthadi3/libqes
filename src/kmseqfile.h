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

#include <kmutil.h>
#include <kmseq.h>
#include <kmzfile.h>

/*---------------------------------------------------------------------------
  |  Seqfile methods -- seamless reading & writing of FASTA & FASTQ         |
  ---------------------------------------------------------------------------*/

#define	FASTA_DELIM '>'
#define	FASTQ_DELIM '@'
#define	FASTQ_QUAL_DELIM '+'

#define	__FASTQ_READ_LINES__ 4
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

seqfile_t *create_seqfile (const char *path, const char *mode);
extern int seqfile_ok(const seqfile_t *file);
seqfile_format_t seqfile_guess_format(seqfile_t *file);
void seqfile_set_format (seqfile_t *file, seqfile_format_t format);
int seqfile_iter_parallel (seqfile_t *file, seqfile_iter_func_t func,
        void *data, seqfile_iter_flags flags);
int seqfile_iter (seqfile_t *file, seqfile_iter_func_t func, void *data,
        seqfile_iter_flags flags);
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
