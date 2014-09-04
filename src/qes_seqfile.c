/*
 * ============================================================================
 *
 *       Filename:  qes_seqfile.c
 *
 *    Description:  qes_seqfile -- read sequences in FASTA or FASTQ format.
 *
 *        Version:  1.0
 *        Created:  14/05/14 15:56:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_seqfile.h"

static inline ssize_t
read_fastq_seqfile(struct qes_seqfile *seqfile, struct qes_seq *seq)
{
    /* Convenience macro, this happens a lot */
#define CHECK_AND_TRIM(subrec) if (len < 1) { \
            goto error;     \
        } else {        \
            subrec.s[--len] = '\0'; \
            subrec.l = len; \
        }
    ssize_t len = 0;
    int next = '\0';
    int errcode = -1;

    /* Fast-forward past the delimiter '@', ensuring it exists */
    next = qes_file_getc(seqfile->qf);
    if (next == EOF) {
        return EOF;
    } else if (next != FASTQ_DELIM) {
        /* This ain't a fastq! WTF! */
        errcode = -2;
        goto error;
    }
    len = qes_file_readline_str(seqfile->qf, &seqfile->scratch);
    if (len < 1) {
        /* Weird truncated file */
        errcode = -3;
        goto error;
    }
    qes_seq_fill_header(seq, seqfile->scratch.s, seqfile->scratch.l);
    /* Fill the actual sequence directly */
    len = qes_file_readline_str(seqfile->qf, &seq->seq);
    errcode = -4;
    CHECK_AND_TRIM(seq->seq)
    /* read the qual header, but don't store it. */
    next = qes_file_getc(seqfile->qf);
    if (next != FASTQ_QUAL_DELIM) {
        errcode = -5;
        goto error;
    }
    while ((next = qes_file_getc(seqfile->qf)) != '\n') {
        if (next == EOF) {
            errcode = -6;
            goto error;
        }
    }
    /* Fill the qual score string directly */
    len = qes_file_readline_str(seqfile->qf, &seq->qual);
    errcode = -7;
    CHECK_AND_TRIM(seq->qual)
    if ((size_t)len != seq->seq.l) {
        /* Error out on different len qual/seq entries */
        errcode = -8;
        goto error;
    }
    /* return seq/qual len */
    seqfile->n_records++;
    return seq->seq.l;
error:
    qes_str_nullify(&seq->name);
    qes_str_nullify(&seq->comment);
    qes_str_nullify(&seq->seq);
    qes_str_nullify(&seq->qual);
    return errcode;
#undef CHECK_AND_TRIM
}

static inline ssize_t
read_fasta_seqfile(struct qes_seqfile *seqfile, struct qes_seq *seq)
{
    /* Convenience macro, this happens a lot */
#define CHECK_AND_TRIM(subrec) if (len < 1) { \
            goto error;     \
        } else {        \
            subrec.s[--len] = '\0'; \
            subrec.l = len; \
        }
    ssize_t len = 0;
    int next = '\0';

    /* This bit is basically a copy-paste from above */
    /* Fast-forward past the delimiter '>', ensuring it exists */
    next = qes_file_getc(seqfile->qf);
    if (next == EOF) {
        return EOF;
    } else if (next != FASTA_DELIM) {
        /* This ain't a fasta! WTF! */
        goto error;
    }
    len = qes_file_readline_str(seqfile->qf, &seqfile->scratch);
    if (len < 1) {
        goto error;
    }
    qes_seq_fill_header(seq, seqfile->scratch.s, seqfile->scratch.l);
    /* we need to nullify seq, as we rely on seq.l being 0 as we enter this
     *  while loop */
    qes_str_nullify(&seq->seq);
    /* While the next char is not a '>', i.e. until next header line */
    while ((next = qes_file_peek(seqfile->qf)) != EOF && next != FASTA_DELIM) {
        len = qes_file_readline(seqfile->qf, seq->seq.s + seq->seq.l,
                seq->seq.m - seq->seq.l - 1);
        if (len < 0) {
            goto error;
        }
        seq->seq.l += len - 1;
        seq->seq.s[seq->seq.l] = '\0';
        if (seq->seq.m -  1 <= seq->seq.l) {
            seq->seq.m = qes_roundupz(seq->seq.m);
            seq->seq.s = qes_realloc(seq->seq.s,
                    sizeof(*seq->seq.s) * seq->seq.m);
            if (seq->seq.s == NULL) {
                goto error;
            }
        }
    }
    seq->seq.s[seq->seq.l] = '\0';
    /* return seq len */
    seqfile->n_records++;
    qes_str_nullify(&seq->qual);
    return seq->seq.l;
error:
    qes_str_nullify(&seq->name);
    qes_str_nullify(&seq->comment);
    qes_str_nullify(&seq->seq);
    qes_str_nullify(&seq->qual);
    return -2;
#undef CHECK_AND_TRIM
}

ssize_t
qes_seqfile_read (struct qes_seqfile *seqfile, struct qes_seq *seq)
{
    if (!qes_seqfile_ok(seqfile) || !qes_seq_ok(seq)) {
        return -2;
    }
    if (seqfile->qf->eof) {
        return EOF;
    }
    if (seqfile->format == FASTQ_FMT) {
        return read_fastq_seqfile(seqfile, seq);
    } else if (seqfile->format == FASTA_FMT) {
        return read_fasta_seqfile(seqfile, seq);
    }
    /* If we reach here, bail out with an error */
    qes_str_nullify(&seq->name);
    qes_str_nullify(&seq->comment);
    qes_str_nullify(&seq->seq);
    qes_str_nullify(&seq->qual);
    return -2;
}

struct qes_seqfile *
qes_seqfile_create (const char *path, const char *mode)
{
    struct qes_seqfile *sf = NULL;
    if (path == NULL || mode == NULL) return NULL;
    sf = qes_calloc(1, sizeof(*sf));
    sf->qf = qes_file_open(path, mode);
    if (sf->qf == NULL) {
        qes_free(sf->qf);
        qes_free(sf);
        return NULL;
    }
    qes_str_init(&sf->scratch, __INIT_LINE_LEN);
    sf->n_records = 0;
    qes_seqfile_guess_format(sf);
    return sf;
}

enum qes_seqfile_format
qes_seqfile_guess_format (struct qes_seqfile *seqfile)
{
    int first_char = '\0';
    if (!qes_seqfile_ok(seqfile)) return UNKNOWN_FMT;
    if (!qes_file_readable(seqfile->qf)) return UNKNOWN_FMT;
    first_char = qes_file_peek(seqfile->qf);
    switch (first_char) {
        case FASTQ_DELIM:
            seqfile->format = FASTQ_FMT;
            return FASTQ_FMT;
            break;
        case FASTA_DELIM:
            seqfile->format = FASTA_FMT;
            return FASTA_FMT;
            break;
        default:
            seqfile->format = UNKNOWN_FMT;
            return UNKNOWN_FMT;
    }
}

void
qes_seqfile_set_format (struct qes_seqfile *seqfile,
                        enum qes_seqfile_format format)
{
    if (!qes_seqfile_ok(seqfile)) return;
    seqfile->format = format;
}

void
qes_seqfile_destroy_(struct qes_seqfile *seqfile)
{
    if (seqfile != NULL) {
        qes_file_close(seqfile->qf);
        qes_str_destroy_cp(&seqfile->scratch);
        qes_free(seqfile);
    }
}

size_t
qes_seqfile_format_seq(const struct qes_seq *seq, enum qes_seqfile_format fmt,
                       char *buffer, size_t maxlen)
{
    size_t len = 0;
    if (buffer == NULL || maxlen < 1) {
        return 0;
    }
    switch (fmt) {
        case FASTQ_FMT:
            if (!qes_seq_ok(seq)) {
                buffer[0] = '\0';
                return 0;
            }
            len = snprintf(buffer, maxlen, "%c%s %s\n%s\n%c\n%s\n",
                    FASTQ_DELIM, seq->name.s, seq->comment.s,
                    seq->seq.s,
                    FASTQ_QUAL_DELIM,
                    seq->qual.s);
            return len;
            break;
        case FASTA_FMT:
            if (!qes_seq_ok_no_qual(seq)) {
                buffer[0] = '\0';
                return 0;
            }
            len = snprintf(buffer, maxlen, "%c%s %s\n%s\n",
                    FASTA_DELIM, seq->name.s, seq->comment.s,
                    seq->seq.s);
            return len;
            break;
        case UNKNOWN_FMT:
        default:
            return 0;
    }
}


ssize_t
qes_seqfile_write (struct qes_seqfile *seqfile, struct qes_seq *seq)
{
#define sf_putc_check(c) ret = QES_ZFPUTC(seqfile->qf->fp, c);                  \
    if (ret != c) {return -2;}                                              \
    else res_len += 1;                                                      \
    ret = 0
#define sf_puts_check(s) ret = QES_ZFPUTS(seqfile->qf->fp, s);                  \
    if (ret < 0) {return -2;}                                               \
    else res_len += ret;                                                    \
    ret = 0

    int ret = 0;
    ssize_t res_len = 0;

    if (!qes_seqfile_ok(seqfile) || !qes_seq_ok(seq)) {
        return -2;
    }
    switch (seqfile->format) {
        case FASTA_FMT:
            sf_putc_check(FASTA_DELIM);
            sf_puts_check(seq->name.s);
            if (qes_seq_has_comment(seq)) {
                sf_putc_check(' ');
                sf_puts_check(seq->comment.s);
            }
            sf_putc_check('\n');
            sf_puts_check(seq->seq.s);
            sf_putc_check('\n');
            break;
        case FASTQ_FMT:
            sf_putc_check(FASTQ_DELIM);
            sf_puts_check(seq->name.s);
            if (qes_seq_has_comment(seq)) {
                sf_putc_check(' ');
                sf_puts_check(seq->comment.s);
            }
            sf_putc_check('\n');
            sf_puts_check(seq->seq.s);
            sf_putc_check('\n');
            if (qes_seq_has_qual(seq)) {
                sf_putc_check('+');
                sf_putc_check('\n');
                sf_puts_check(seq->qual.s);
                sf_putc_check('\n');

            }
            break;
        case UNKNOWN_FMT:
        default:
            return -2;
            break;
    }
    return res_len;
#undef sf_putc_check
#undef sf_puts_check
}
