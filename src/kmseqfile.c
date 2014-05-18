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

inline ssize_t
read_seqfile (seqfile_t *file, seq_t *seq)
{
    if (!zfile_ok(file->zf)) return -2;
    if (file->zf->eof) return -1;

    if (file->flags.format == FASTQ_FMT) {
        size_t size = __INIT_LINE_LEN;
        char *header = km_calloc(size, sizeof(*header));
        size_t len = 0;
        ssize_t seqlen = 0;
        len = zfreadline_realloc(file->zf, &header, &size);
        if (len < 1) {
            km_free(header);
            return len; /* deal with error/EOF */
        }
        /* Fill name and comment of seq, dealing w/ errors */
        if (header[0] != FASTQ_DELIM) return -2;
        if (!seq_fill_header(seq, header, len)) return -2;
        /* Fill the actual sequence directly */
        len = zfreadline_str(file->zf, &seq->seq);
        if (len < 1) {
            km_free(header);
            return len; /* deal with error/EOF */
        }
        seq->seq.s[--len] = '\0'; /* remove newline AND --len */
        seqlen = len;
        /* read the qual header into `header` */
        len = zfreadline_realloc(file->zf, &header, &size);
        if (len < 1) {
            km_free(header);
            return len; /* deal with error/EOF */
        }
        if (header[0] != FASTQ_QUAL_DELIM) return -2;
        /* Fill the actual sequence directly */
        len = zfreadline_str(file->zf, &seq->qual);
        if (len < 1) {
            km_free(header);
            return len; /* deal with error/EOF */
        }
        seq->qual.s[--len] = '\0'; /* remove newline AND --len */
        km_free(header);
        if (km_unlikely(len != seqlen)) {
            return -2; /* Error out on different len qual/seq entries */
        }
        return len; /* return seq/qual len */
    } else if (file->flags.format == FASTA_FMT) {
        size_t size = __INIT_LINE_LEN;
        char *buf = km_calloc(size, sizeof(*buf));
        size_t len = 0;
        int next = '\0';
        /* Read in header line */
        len = zfreadline_realloc(file->zf, &buf, &size);
        if (len < 1) {
            km_free(buf);
            return len; /* deal with error/EOF */
        }
        /* Fill name and comment of seq, dealing w/ errors */
        if (buf[0] != FASTA_DELIM) return -2;
        if (!seq_fill_header(seq, buf, len)) return -2;
        /* While the next char is not a '>', i.e. until next header line */
        len = 0;
        while ((next = KM_ZFGETC(file->zf->fp)) != FASTA_DELIM && next != EOF) {
            if (next != '\n') {
                if (km_unlikely(len + 1 >= size - 1)) {
                    size = kmroundupz(size);
                    buf = km_realloc(buf, sizeof(*buf) * size);
                }
                buf[len++] = next;
            }
        }
        buf[len] = '\0';
        seq_fill_seq(seq, buf, len);
        km_free(buf);
        return len;
    }
    return -2; /* If we reach here, bail out with an error */
}

seqfile_t *
create_seqfile (const char *path, const char *mode)
{
    seqfile_t *sf = NULL;
    if (km_unlikely(path == NULL || mode == NULL)) return NULL;
    sf = km_calloc(1, sizeof(*sf));
    sf->zf = zfopen(path, mode);
    sf->n_records = 0llu;
    if (km_unlikely(sf->zf == NULL)) return NULL;
    seqfile_guess_format(sf);
    return sf;
}
int
seqfile_guess_format (seqfile_t *file)
{
    int first_char = '\0';
    if (km_unlikely(!seqfile_ok(file))) return UNKNOWN_FMT;
    if (km_unlikely(file->zf->filepos != 0)) return UNKNOWN_FMT;
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

inline int
seqfile_ok(const seqfile_t *file)
{
    return (file != NULL && zfile_ok(file->zf));
}

void
destroy_seqfile_(seqfile_t *seqfile)
{
    if (seqfile != NULL) {
        zfclose(seqfile->zf);
    }
}
