/*
 * ============================================================================
 *
 *       Filename:  kmzfile.c
 *
 *    Description:  Compressed file IO
 *
 *        Version:  1.0
 *        Created:  09/05/14 12:20:57
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "kmzfile.h"

zfile_t *
zfopen_ (const char *path, const char *mode, errhandler_t onerr,
        const char *file, int line)
{
    /* create file struct */
    zfile_t *zf = NULL;
    if (path == NULL || mode == NULL) return NULL;
    zf = km_calloc(1, sizeof(*zf));
    /* Open file, handling any errors */
    zf->fp = KM_ZOPEN(path, mode);
    if (zf->fp == NULL) {
        (*onerr)("Opening file %s failed:\n%s\n", file, line,
                path, strerror(errno));
        km_free(zf);
        return(NULL);
    }
    zf->mode = zfile_guess_mode(mode);
    /* Use a larger than default IO buffer, speeds things up */
    KM_ZBUFFER(zf->fp, KM_FILEBUFFER_LEN);
    /* init struct fields */
    zf->eof = 0;
    zf->filepos = 0;
    zf->path = strndup(path, KM_MAX_FN_LEN);
    return(zf);
}

inline int
zfile_guess_mode (const char *mode)
{
    if (mode[0] == 'r') {
        return RW_READ;
    } else if(mode[0] == 'w' || mode[0] == 'a') {
        return RW_WRITE;
    }
    return RW_UNKNOWN;
}

inline void
zfrewind (zfile_t *file)
{
    if (zfile_ok(file)) {
        KM_ZSEEK(file->fp, 0, SEEK_SET);
        file->filepos = 0;
        file->eof = 0;
    }
}

void
zfclose_ (zfile_t *file)
{
    if (file != NULL) {
        if (file->fp != NULL) {
            KM_ZCLOSE(file->fp);
        }
        km_free(file->path);
        km_free(file);
    }
}

inline ssize_t
zfreadline_realloc_ (zfile_t *file, char **buf, size_t *size,
        errhandler_t onerr, const char *src, const int line)
{
    return zfgetuntil_realloc_(file, '\n', buf, size, onerr, src, line);
}

inline ssize_t
zfgetuntil_realloc_ (zfile_t *file, int delim, char **bufref, size_t *sizeref,
        errhandler_t onerr, const char *src, const int line)
{
    size_t len = 0;
    int next = 0;
    char *buf = NULL;
    size_t size = 0;
    if (bufref == NULL || file == NULL || sizeref == NULL) {
        return -2; /* eof is normally == -1, so use -2 to differentiate them */
    }
    if (file->eof) return EOF;
    buf = *bufref;
    size = *sizeref;
    /* Alloc the buffer if it's NULL */
    if (buf == NULL) {
        buf = km_malloc_(__INIT_LINE_LEN * sizeof(*buf), onerr, src, line);
        size = __INIT_LINE_LEN;
    }
    while((next = KM_ZFGETC(file->fp)) != EOF) {
        /* Make sure we have space for both ``next``, the char we just read,
         * and a terminating '\0' (hence len + 2) */
        while (len + 2 >= size) {
            size = kmroundupz((size) + 1);
            buf = km_realloc_(buf, sizeof(*buf) * size, onerr, src, line);
            if (buf == NULL) {
                /* We bail out here, and *bufref is untouched. This means we
                 * can check for errors, and free *bufref from the calling
                 * function */
                return -2;
            }
        }
        buf[len++] = next;
        if (next == delim) break;
    }
    buf[len] = '\0';
    file->filepos += len;
    *bufref = buf;
    *sizeref = size;
    if (len) {
        if (KM_ZEOF(file->fp)) {
            file->eof = 1;
        }
        return len;
    } else {
        file->eof = 1;
        return EOF;
    }
}
inline ssize_t
zfgetuntil (zfile_t *file, const int delim, char *dest, size_t maxlen)
{
    size_t len = 0;
    int next = 0;
    if (dest == NULL || file == NULL || maxlen < 1 || delim > 255) {
        /* EOF is normally == -1, so use -2 to differentiate them */
        return -2;
    }
    /* Get out early if we're at EOF already */
    if (file->eof) return EOF;
    /* maxlen - 1 because we always leave space for \0 */
    while ( (next = KM_ZFGETC(file->fp)) != EOF && len < maxlen - 1) {
        dest[len++] = next;
        if (next == delim) break;
    }
    dest[len] = '\0';
    file->filepos += len;
    if (len) {
        if (KM_ZEOF(file->fp)) {
            file->eof = 1;
        }
        return len;
    } else {
        file->eof = 1;
        return EOF;
    }
}

inline ssize_t
zfreadline (zfile_t *file, char *dest, size_t maxlen)
{
    return zfgetuntil(file, '\n', dest, maxlen);
}

inline ssize_t
zfreadline_str (zfile_t *file, str_t *str)
{
    if (file == NULL || str_ok(str)) {
        return -2; /* ERROR, not EOF */
    }
    return zfreadline_realloc(file, &str->s, &str->m);
}

inline void
zfprint_str (zfile_t *stream, const str_t *str)
{
    KM_ZWRITE(stream->fp, str->s, str->l);
}

inline int
zfile_ok(const zfile_t *zf)
{
    return  zf != NULL && \
            zf->fp != NULL;
}

inline int
zfile_readable(const zfile_t *zf)
{
    return zfile_ok(zf) && !zf->eof;

}

inline int
zfpeek (zfile_t *file)
{
    int chr = '\0';
    if (km_unlikely(!zfile_ok(file))) return -2;
    chr = KM_ZFGETC(file->fp);
    KM_ZFUNGETC(chr, file->fp);
    return chr;
}
