/*
 * ============================================================================
 *
 *       Filename:  kmfile.c
 *
 *    Description:  Generic text file IO for sequence data
 *
 *        Version:  1.0
 *        Created:  02/12/13 17:19:07
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */

#include "kmfile.h"


inline void
__kmfile_fill_buffer (file_t *file)
{
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
    * After a read. Needs refilling, but have to shuffle first:
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

    size_t bytes_to_read = 0;
    size_t bytes_to_move = 0;
    int bytes_read = 0;

    if (file == NULL) {
        return;
    }

    /* we need to read into buffer, from bufferiter to bufferend. */
    bytes_to_read = KMFILE_BUFFER_LEN - \
                    (size_t)(file->bufferend - file->bufferiter) - 1;

    /* printf("at start of fill buffer.\n\tbf: %p, bi %p, be: %p, btr: %zu\n",
            file->buffer, file->bufferiter, file->bufferend, bytes_to_read); */
    if (bytes_to_read < 1) {
        /*  Already full! */
        /* KM_ERROR("file_t already full") */
        return;
    }

    /* shuffle the buffer */
    /* we don't do the customary bufflen -1 here, as we want to copy the \0
    *  which should always be the final char
    */
    bytes_to_move = KMFILE_BUFFER_LEN - \
                    ((size_t) file->bufferiter - (size_t)file->buffer);
    /* printf("moving buffer. bf: %p, bi %p, be %p, btm, %zu\n",
            file->buffer, file->bufferiter, file->bufferend, bytes_to_move); */
    if(bytes_to_move > 0) {
        memmove(file->buffer, file->bufferiter, bytes_to_move);
    }

    /* we've moved the iterator to the start, update pointers */
    file->bufferiter = file->buffer;
    file->bufferend = file->buffer + bytes_to_move;
    /* And bzero out the empty bit of buffer */
    bzero(file->bufferend, KMFILE_BUFFER_LEN - bytes_to_move);

    /* printf("after move: %p, bi %p, be, %p\n",
            file->buffer, file->bufferiter, file->bufferend); */
    /* Refill the buffer */
    bytes_read = KM_ZREAD(file->file, file->bufferend - 1, bytes_to_read);
    if (bytes_read < bytes_to_read) { /* check for errors */
        if (gzeof(file->file)) {
            file->eof = 1;
        }
        else {
            const char * err_str = KM_ZERR(file->file, &file->err);
            if (file->err) {
                fprintf(stderr, "Error reading %s:\n%s\n",
                        file->path, err_str);
                exit(EXIT_FAILURE);
            }
        }
    }
    /* printf("after refill: %p, bi %p, be, %p\n",
            file->buffer, file->bufferiter, file->bufferend); */
    file->filepos += bytes_to_read;
    /* reset pointers */
    file->bufferend = file->buffer + bytes_read + 1;
    file->bufferiter = file->buffer;
    /* printf("finished: %p, bi %p, be, %p\n",
            file->buffer, file->bufferiter, file->bufferend); */
}


file_t *
create_file_t (const char *filename)
{
    size_t      fnlen;
    file_t     *file;
    /* Make struct */
    file = calloc(1, sizeof(*file));
    CHECK_ALLOC(file);

    /* init file */
    file->file = KM_ZOPEN(filename, "r");
    if (file->file == NULL) {
        fprintf(stderr, "Opening file %s failed:\n%s\n",
                filename, strerror(errno));
        km_free(file);
        return(NULL);
    }

    /* init eof */
    file->eof = 0;

    /* init path */
    fnlen = strlen(filename) + 1;

    file->path = calloc(fnlen, sizeof(char));
    CHECK_ALLOC(file->path);
    strncpy(file->path, filename, fnlen);

    /* init buffer */
    file->buffer = calloc(KMFILE_BUFFER_LEN, sizeof(char));
    CHECK_ALLOC(file->buffer);

    /* set iteration pointers */
    file->bufferend = file->buffer + (size_t)(KMFILE_BUFFER_LEN - 1);
    file->bufferiter = file->bufferend;

    __file_t_fill_buffer(file);

    file->mode = file->bufferiter[0];
    /* printf("file mode is %c\n", file->mode); */
    return(file);
}

void
destroy_file_t (file_t *file)
{
    if(file != NULL){
        if (file->file != NULL) { KM_ZCLOSE(file->file); }
        km_free(file->buffer);
        km_free(file->path);
        km_free(file);
    }
}

inline size_t
__readline_file_t_keep (file_t *file, char **dest, size_t maxlen)
{
    char *newline = NULL;
    size_t len = 0;

    /* printf("readling line (inner)\nf: %p, d: %p, m %zu\n",
            file, dest, maxlen); */
    newline = strchr(file->bufferiter, '\n');
    /* printf("nl %p, bi %p, %s, %i %i\n", newline, file->bufferiter,
            file->bufferiter, *(file->bufferiter), *(file->bufferiter+1)); */
    if (newline == NULL) {
        /* printf("no newline found, filling buffer\n"); */
        __file_t_fill_buffer(file);
        newline = strchr(file->bufferiter, '\n');
    }

    /* printf("nl %p, bi %p, %s, %i %i\n", newline, file->bufferiter,
            file->bufferiter, *(file->bufferiter), *(file->bufferiter+1)); */
    len = (newline - file->bufferiter) + 1;
    if (len >= maxlen) len = maxlen - 1;

    /* copy to destination  */
    strncpy(*dest, file->bufferiter, len);
    (*dest)[len] ='\0';

    return len;
}

size_t
readline_file_t (file_t *file, char **dest, size_t maxlen)
{
    size_t bytes_taken;

    bytes_taken = __readline_file_t_keep(file, dest, maxlen);
    /* don't keep the bytes in the buffer's todo section  */
    file->bufferiter += bytes_taken;
    return bytes_taken;
}

inline size_t
hint_line_length_file_t (file_t *file)
{
    size_t len = 0;
    char *tmp;
    tmp = strchr(file->bufferiter, '\n');
    return tmp - file->bufferiter + 2;
}

inline char
peek_ahead_file (file_t *file)
{
    return file->bufferiter[0];
}

/* Zero-copy `getline()`, with the added benefit of `realloc`-ing `buf` to
    the next highest base-2 power, if we run out of space. If it is realloced,
    (*size) is updated to the right size.
    DON'T USE ON STACK BUFFERS.
*/
static inline ssize_t
km_readline_realloc_ (char **buf, FILE *fp, size_t *size, errhandler_t onerr,
        char *file, int line)
{
    if (buf == NULL || (*buf) == NULL || fp == NULL || size == NULL) {
        return -2; /* EOF is normally == -1, so use -2 to differentiate them */
    }
    size_t len = 0;
    while(((*buf)[len] = fgetc(fp)) != EOF && (*buf)[len++] != '\n') {
        while (len + 1 >= (*size) - 1) {
            size_t newsize = (*size) + 1;
            *size = kmroundupz(newsize);
            char *newbuf = km_realloc(*buf, sizeof(**buf) * (*size), onerr);
            if (newbuf == NULL) {
                return -2;
            } else (*buf) = newbuf;
        }
    }
    (*buf)[len] = '\0';
    if (feof(fp)) return EOF;
    else return len;
}
