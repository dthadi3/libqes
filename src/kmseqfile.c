/*
 * ============================================================================
 *
 *       Filename:  kmseqfile.c
 *
 *    Description:  kmseqfile -- read sequences in FASTA or FASTQ format.
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

#include "kmseqfile.h"

static inline ssize_t
read_fastq_seqfile(seqfile_t *file, seq_t *seq)
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
    /* Fast-forward past the delimiter '@', ensuring it exists */
    next = zfgetc(file->zf);
    if (next == EOF) {
        return EOF;
    } else if (next != FASTQ_DELIM) {
        /* This ain't a fastq! WTF! */
        goto error;
    }
    /* Get until the first space, which is the seq name */
    len = zfgetuntil_realloc(file->zf, ' ', &seq->name.s, &seq->name.m);
    CHECK_AND_TRIM(seq->name)
    /* Fill the comment, from first space to EOL */
    len = zfreadline_realloc(file->zf, &seq->comment.s, &seq->comment.m);
    CHECK_AND_TRIM(seq->comment)
    /* Fill the actual sequence directly */
    len = zfreadline_str(file->zf, &seq->seq);
    CHECK_AND_TRIM(seq->seq)
    /* read the qual header, but don't store it. */
    next = zfgetc(file->zf);
    if (next != FASTQ_QUAL_DELIM) {
        goto error;
    }
    while ((next = zfgetc(file->zf)) != '\n') {
        if (next == EOF) {
            goto error;
        }
    }
    /* Fill the qual score string directly */
    len = zfreadline_str(file->zf, &seq->qual);
    CHECK_AND_TRIM(seq->qual)
    if ((size_t)len != seq->seq.l) {
        /* Error out on different len qual/seq entries */
        goto error;
    }
    /* return seq/qual len */
    return seq->seq.l;
error:
    str_nullify(&seq->name);
    str_nullify(&seq->comment);
    str_nullify(&seq->seq);
    str_nullify(&seq->qual);
    return -2;
#undef CHECK_AND_TRIM
}

static inline ssize_t
read_fasta_seqfile(seqfile_t *file, seq_t *seq)
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
    next = zfgetc(file->zf);
    if (next == EOF) {
        return EOF;
    } else if (next != FASTA_DELIM) {
        /* This ain't a fasta! WTF! */
        goto error;
    }
    /* Get until the first space, which is the seq name */
    len = zfgetuntil_realloc(file->zf, ' ', &seq->name.s, &seq->name.m);
    CHECK_AND_TRIM(seq->name)
    /* Fill the comment, from first space to EOL */
    len = zfreadline_realloc(file->zf, &seq->comment.s, &seq->comment.m);
    CHECK_AND_TRIM(seq->comment)
    /* End fastq parser copypaste */
    /* we need to nullify seq, as we rely on seq.l being 0 as we enter this
     *  while loop */
    str_nullify(&seq->seq);
    /* While the next char is not a '>', i.e. until next header line */
    while ((next = zfpeek(file->zf)) != EOF && next != FASTA_DELIM) {
        len = zfreadline(file->zf, seq->seq.s + seq->seq.l,
                seq->seq.m - seq->seq.l - 1);
        if (len < 0) {
            goto error;
        }
        seq->seq.l += len - 1;
        seq->seq.s[seq->seq.l] = '\0';
        if (seq->seq.m -  1 <= seq->seq.l) {
            seq->seq.m = kmroundupz(seq->seq.m);
            seq->seq.s = km_realloc(seq->seq.s,
                    sizeof(*seq->seq.s) * seq->seq.m);
            if (seq->seq.s == NULL) {
                goto error;
            }
        }
    }
    seq->seq.s[seq->seq.l] = '\0';
    /* return seq len */
    str_nullify(&seq->qual);
    return seq->seq.l;
error:
    str_nullify(&seq->name);
    str_nullify(&seq->comment);
    str_nullify(&seq->seq);
    str_nullify(&seq->qual);
    return -2;
#undef CHECK_AND_TRIM
}
inline ssize_t
seqfile_read (seqfile_t *file, seq_t *seq)
{
    if (!seqfile_ok(file) || !seq_ok(seq)) {
        return -2;
    }
    if (file->zf->eof) {
        return EOF;
    }
    if (file->flags.format == FASTQ_FMT) {
        return read_fastq_seqfile(file, seq);
    } else if (file->flags.format == FASTA_FMT) {
        return read_fasta_seqfile(file, seq);
    }
    /* If we reach here, bail out with an error */
    str_nullify(&seq->name);
    str_nullify(&seq->comment);
    str_nullify(&seq->seq);
    str_nullify(&seq->qual);
    return -2;
}

seqfile_t *
seqfile_create (const char *path, const char *mode)
{
    seqfile_t *sf = NULL;
    if (path == NULL || mode == NULL) return NULL;
    sf = km_calloc(1, sizeof(*sf));
    sf->zf = zfopen(path, mode);
    if (sf->zf == NULL) {
        km_free(sf->zf);
        km_free(sf);
        return NULL;
    }
    sf->n_records = 0;
    seqfile_guess_format(sf);
    return sf;
}

seqfile_format_t
seqfile_guess_format (seqfile_t *file)
{
    int first_char = '\0';
    if (!seqfile_ok(file)) return UNKNOWN_FMT;
    if (!zfile_readable(file->zf)) return UNKNOWN_FMT;
    first_char = zfpeek(file->zf);
    switch (first_char) {
        case FASTQ_DELIM:
            file->flags.format = FASTQ_FMT;
            return FASTQ_FMT;
            break;
        case FASTA_DELIM:
            file->flags.format = FASTA_FMT;
            return FASTA_FMT;
            break;
        default:
            file->flags.format = UNKNOWN_FMT;
            return UNKNOWN_FMT;
    }
}

void
seqfile_set_format (seqfile_t *file, seqfile_format_t format)
{
    if (!seqfile_ok(file)) return;
    file->flags.format = format;
}

inline int
seqfile_ok(const seqfile_t *file)
{
    return (file != NULL && zfile_ok(file->zf));
}

void
seqfile_destroy_(seqfile_t *seqfile)
{
    if (seqfile != NULL) {
        zfclose(seqfile->zf);
        km_free(seqfile);
    }
}

inline size_t
seqfile_format_seq(const seq_t *seq, seqfile_format_t fmt, char *buffer,
        size_t maxlen)
{
    size_t len = 0;
    if (buffer == NULL || maxlen < 1) {
        return 0;
    }
    switch (fmt) {
        case FASTQ_FMT:
            if (!seq_ok(seq)) {
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
            if (!seq_ok_no_qual(seq)) {
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

inline ssize_t
seqfile_write (seqfile_t *file, seq_t *seq)
{
    ssize_t len = 0;
    const size_t buflen = 1<<12; /* 4k max seq len, should be plenty */
    char buffer[1<<12];
    int ret = 0;

    if (!seqfile_ok(file) || !seq_ok(seq)) {
        return -2;
    }
    len = seqfile_format_seq(seq, file->flags.format, buffer, buflen);
    if ((ssize_t) buflen <= len) {
        return -2;
    }
    ret = KM_ZWRITE(file->zf->fp, buffer, len);
    if (ret < len) {
        return -2;
    }
    return len;
}
