/*
 * =====================================================================================
 *
 *       Filename:  kmseq.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  31/07/13 19:53:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <zlib.h>

#include "kmseq.h"


inline void __kmfile_fill_buffer(kmfile * file) {
    /* work out how many bytes we need
    * This needs a picture
    *  At each time
    *    BUFLEN = 14
    *    buf = ^
    *    bufiter = i
    *    bufend = !
    *    pos. of first record = R
    *
    * Before a read:
    *  < >< >< >< >< >< >< >< >< >< >< >< >< >< > <- bytes
    *   0  1  2  3  4  5  6  7  8  9 10 11 12 13
    *   @  1 \n  A \n  + \n  # \n  @  2 \n  A \0
    *  R^i                                     !
    *
    * After a read. Needs refilling, but have to shuffle first
    *  < >< >< >< >< >< >< >< >< >< >< >< >< >< > <- bytes
    *   0  1  2  3  4  5  6  7  8  9 10 11 12 13
    *   @  1 \n  A \n  + \n  # \n  @  2 \n  A \0
    *   ^                          Ri          !
    *
    * After a shuffle
    *  < >< >< >< >< >< >< >< >< >< >< >< >< >< > <- bytes
    *   0  1  2  3  4  5  6  7  8  9 10 11 12 13
    *   @  2 \n  A \0  + \n  # \n  @  2 \n  A \0
    *  R^i          !
    *
    *  bytes_to_read = 13 - 4
    *                = buf + ((bufflen - 1) - strlen(buf))
    * After a refill
    *  < >< >< >< >< >< >< >< >< >< >< >< >< >< > <- bytes
    *   0  1  2  3  4  5  6  7  8  9 10 11 12 13
    *   @  2 \n  A \n  + \n  # \n  @  3 \n  G \0
    *  R^i                                     !
    *
    */

    size_t bytes_to_read;
    bytes_to_read = KMFILE_BUFFER_LEN - (size_t)(file->bufferend - file->bufferiter) - 1;

    if (bytes_to_read < 1) {
        /*  Already full! */
        return;
    }

    /* shuffle the buffer */
    /* we don't do the customary bufflen -1 here, as we want to copy the \0 which
    *  should always be the final char
    */
    size_t bytes_to_move = (KMFILE_BUFFER_LEN - (size_t) file->bufferiter);
    printf("moving buffer. bf: %x, bi %x, btm, %i\n",
            file->buffer, file->bufferiter, bytes_to_move);
    memmove(file->buffer, file->bufferiter, bytes_to_move);

    /* Refill the buffer */
    int bytes_read = gzread(file->file, file->bufferiter, bytes_to_read);
    if (bytes_read < bytes_to_read) { /* check for errors */
        if (gzeof(file->file)) {
            file->eof = 1;
        }
        else {
            const char * err_str = gzerror(file->file, &file->err);
            if (file->err) {
                fprintf(stderr, "Error reading %s:\n%s\n",
                        file->path, err_str);
                exit(EXIT_FAILURE);
            }
        }
    }

    file->filepos += bytes_to_read;

    /* reset pointers */
    file->bufferiter = file->buffer;
    file->bufferend = file->buffer + (KMFILE_BUFFER_LEN - 1);
}


kmfile *create_kmfile(const char * filename){
    /* Make struct */
    kmfile *file = (kmfile *) calloc(1, sizeof(*file));
    CHECK_ALLOC(file);

    /* init file */
    file->file = gzopen(filename, "r");
    if (!file->file) {
        fprintf(stderr, "Opening file %s failed:\n%s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* init eof */
    file->eof = 0;

    /* init path */
    int fnlen = strlen(filename) + 1;
    file->path = (char *) calloc(fnlen, sizeof(char));
    CHECK_ALLOC(file->path);
    strncpy(file->path, filename, fnlen - 1);

    /* init buffer */
    file->buffer = (char *) calloc(KMFILE_BUFFER_LEN, sizeof(char));
    CHECK_ALLOC(file->buffer);

    /* set iteration pointers */
    file->bufferiter = file->buffer;
    file->bufferend = file->buffer + (KMFILE_BUFFER_LEN - 1);

    __kmfile_fill_buffer(file);
    return(file);
}

void destroy_kmfile(kmfile *file) {
    if(file){
        FREE(file->buffer);
        FREE(file->path);
        FREE(file);
    }
}

kmseq *read_seq_file(kmfile *file){
    /* in case someone tries to get seq from uninitialised kmfile */
    if (!file) return NULL;
    /* in case someone tries getting seqs from a file at EOF  */
    if (file->eof) return NULL;

}

kmseq *init_kmseq() {
    kmseq *seq = (kmseq *) calloc(1, sizeof(*seq));

    seq->name = (char *) calloc(KMSEQ_HEADER_LEN, 1);
    seq->seq = (char *) calloc(KMSEQ_SEQ_LEN, 1);
#ifdef  KM_QUAL_H
    seq->qual_h = (char *) calloc(KMSEQ_HEADER_LEN, 1);
#endif     /* -----  not KM_QUAL_H  ----- */
    seq->qual = (char *) calloc(KMSEQ_SEQ_LEN, 1);

    return seq;
}

void fill_kmseq(kmseq *seqref, const char *name, const char *seq, const char *qual_h, const char *qual) {
    int name_l, seq_l, qual_h_l, qual_l;


}
void destroy_kmseq(kmseq *seq) {
    if (seq) {
        FREE(seq->name);
        FREE(seq->seq);
        FREE(seq->qual_h);
        FREE(seq->qual);
        FREE(seq);
    }
}
