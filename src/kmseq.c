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
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */

#include "kmseq.h"

#define	__READ_LINES	4
seq_t *
read_seq_file (seqfile_t *file)
{
    seq_t *seq = NULL;
    int len_tmp = 0;
    size_t bytes_read = 0;
    size_t seqlen = 0;
    size_t alloced_lines = 0;
    /* in case someone tries to get seq from uninitialised kmfile */
    if (file == NULL) return NULL;
    /* in case someone tries getting seqs from a file at EOF  */
    if (file->zf->eof) return NULL;

    if (file->flags->format == FASTQ) {
        /* Fastq files always have 4 lines. A simple for loop will do. */
        char *lines[4];
        size_t line = 0;
        for (line = 0; line < 4; line++) {
            lines[line] = km_calloc(__INIT_LINE_LEN, sizeof(*buf));
            zfreadline_realloc(
        }
    } else if (file->mode == '>') {
        /* Start with two lines, for shits and giggles. */
        alloced_lines = 2;
        lines = calloc(alloced_lines, sizeof(*lines));

        /* Grab first line or next while loop conditional will be false. */
        /* follows same pattern as above */
        len_tmp = hint_line_length_kmfile(file);
        lines[line] = calloc(len_tmp + 1, sizeof(**lines));
        bytes_read = readline_kmfile(file, &lines[line], len_tmp);
        lines[line][bytes_read-1] = '\0';

        /* while the next char is not a '>', i.e. until next header line */
        while (peek_ahead_kmfile(file) != file->mode) {
            /* do the log-incrementation of array if need be */
            if (line + 1 >= alloced_lines) {
                /* printf("reallocing from %i to %i as %i is >=\n",
                        alloced_lines, alloced_lines << 1, line +1); */
                alloced_lines = alloced_lines << 1;
                lines = realloc(lines, sizeof(*lines) * alloced_lines);
            }

            /* grab line, per above. You should be used to what this does by
               now */
            len_tmp = hint_line_length_kmfile(file);
            lines[++line] = calloc(len_tmp + 1, sizeof(**lines));
            bytes_read = readline_kmfile(file, &lines[line], len_tmp);
            lines[line][bytes_read-1] = '\0';

            /* we're always in sequence by the time it gets to here */
            seqlen += bytes_read - 1; /* remove the \n from the count */
        }
        /* not +1 here, as the ++line above increments it after the final
           cycle is done, so it acutally is the number of lines */
        n_lines = line + 1;
    }

    if (file->mode == '@') {
        /* This bit is simple, add the lines to the seq object.
           It's lines[0]+1, as we want to get rid of the @/> */
#ifdef KMSEQ_QUAL
        fill_kmseq(seq, lines[0]+1, lines[1], lines[3]);
#else
        fill_kmseq(seq, lines[0]+1, lines[1], NULL);
#endif
        seq->seqlen = seqlen;
    } else if (file->mode == '>') {
        /* we need to have somewhere to hold the seq as we stick it together */
        char *tmp = calloc(seqlen+1, sizeof(*tmp));
        size_t copied = 0;

        /* Process each line */
        for (int iii = 1; iii < n_lines; iii++) {
            /* printf("adding subseq '%s' to tmp (%s)\n", lines[iii], tmp);
            printf("to %p, from %p, count %zu\n", tmp + copied - 1, lines[iii],
                    seqlen - copied); */
            strncpy(tmp + copied, lines[iii], seqlen - copied);
            copied = strlen(tmp);
            /* printf("tmp is now '%s'\n", tmp); */
        }
        /* fill in the seq as above. */
        fill_kmseq(seq, lines[0]+1, tmp, NULL);

        FREE(tmp);
    }

    if (lines != NULL ) {
        for (int iii = 0; iii < n_lines; iii++) {
            if (lines[iii] != NULL) { FREE(lines[iii]); }
        }
        FREE(lines);
    }
    return seq;
}

kmseq *
create_kmseq ()
{
    kmseq *seq = calloc(1, sizeof(*seq));
    return seq;
}

void
print_kmseq (kmseq const *seq, FILE *stream)
{
    fprintf(stream, "kmseq at %p\n", (void *)seq);
    fprintf(stream, "\t%s\n", seq->name);
    fprintf(stream, "\t%s\n", seq->seq);
#ifdef  KMSEQ_QUAL
    fprintf(stream, "\t%s\n", seq->qual);
#endif
}

int
fill_kmseq (kmseq *seqref, const char *name, const char *seq, const char *qual)
{
    if (name) {seqref->name = strdup(name);}
    if (seq) {seqref->seq = strdup(seq); seqref->seqlen = strlen(seq);}
#ifdef  KMSEQ_QUAL
    if (qual) {
        seqref->qual = strdup(qual);
        if (strlen(qual) != strlen(seq)) {
            KM_ERROR("seq and qual lengths are not equal");
            FREE(seqref->name);
            FREE(seqref->seq);
            return 1;
        }
    }
#endif

    return 0;
}

void
destroy_kmseq (kmseq *seq)
{
    if (seq != NULL) {
        FREE(seq->name);
        FREE(seq->seq);
#ifdef  KMSEQ_QUAL
        FREE(seq->qual);
#endif
        FREE(seq);
        seq = NULL;
    }
}
