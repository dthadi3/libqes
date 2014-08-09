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
    if (zf->mode == RW_UNKNOWN) {
        KM_ZCLOSE(zf->fp);
        km_free(zf);
        return NULL;
    }
    /* Use a larger than default IO buffer, speeds things up.
     * Using 2x our buffer len for no particular reason. */
    KM_ZBUFFER(zf->fp, (KM_FILEBUFFER_LEN) << 1);
    if (zf->mode == RW_READ || zf-> mode == RW_READWRITE) {
#ifdef HAVE_POSIX_MEMALIGN
        posix_memalign((void *)&(zf->buffer), getpagesize(),
                (KM_FILEBUFFER_LEN * sizeof(*zf->buffer)));
        if (zf->buffer == NULL) {
            KM_ZCLOSE(zf->fp);
            km_free(zf);
            (*onerr)("Coudn't allocate aligned memory", file, line);
            return NULL;
        }
#else
        zf->buffer = km_malloc_((KM_FILEBUFFER_LEN * sizeof(*zf->buffer)),
                onerr, file, line);
        if (zf->buffer == NULL) {
            KM_ZCLOSE(zf->fp);
            km_free(zf);
            (*onerr)("Coudn't allocate aligned memory", file, line);
            return NULL;
        }
#endif
        zf->bufiter = zf->buffer;
        zf->buffer[0] = '\0';
        zf->bufend = zf->buffer;
        if (__zfile_fill_buffer(zf) == 0) {
            km_free(zf->buffer);
            km_free(zf);
            return NULL;
        }
    }
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
        file->feof = 0;
        file->bufiter = file->buffer;
        file->bufend = file->buffer;
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
        km_free(file->buffer);
        file->bufiter = NULL;
        file->bufend = NULL;
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
    size_t tocpy = 0;
    char *buf = NULL;
    char *nextbuf = NULL;
    char *end = NULL;
    size_t size = 0;
    int ret = 0;
    if (bufref == NULL || !zfile_ok(file) || sizeref == NULL) {
        return -2;
    }
    if (file->eof) {
        return EOF;
    }
    /* store local copies to stop dereferencing every time we need them */
    buf = *bufref;
    size = *sizeref;
    /* Alloc the buffer if it's NULL */
    if (buf == NULL) {
        buf = km_malloc_(__INIT_LINE_LEN * sizeof(*buf), onerr, src, line);
        size = __INIT_LINE_LEN;
        buf[0] = '\0';
    }
    /* Set nextbuf AFTER we may/may not have alloced buf above */
    nextbuf = buf;
    /* In case we error out below, we always set bufref = buf here, as
       then we don't lose the memory alloced above */
    *bufref = buf;
    /* Read until delim is in file->buffer, filling buffer */
    while ((end = strchr(file->bufiter, delim)) == NULL) {
        /* copy the remainder of the buffer */
        tocpy = file->bufend - file->bufiter;
        len += tocpy;
        while (len + 1 >= size) {
            size = kmroundupz(size);
            buf = km_realloc_(buf, sizeof(*buf) * size, onerr, src, line);
            if (buf == NULL) {
                /* We bail out here, and *bufref is untouched. This means we
                 * can check for errors, and free *bufref from the calling
                 * function */
                return -2;
            }
        }
        /* set to the correct position in the NEW buf, maybe after realloc */
        nextbuf = buf + len - tocpy;
        memcpy(nextbuf, file->bufiter, tocpy);
        /* Update pointers to point to their respective heads */
        nextbuf += tocpy;
        file->bufiter += tocpy;
        /* Null-terminate buf */
        buf[len] = '\0';
        /* file->buffer should now be empty, so fill 'er up! */
        ret = __zfile_fill_buffer(file);
        if (ret == 0) {
            /* Couln't fill, error out */
            return -2;
        } else if (ret == EOF) {
            /* EOF time */
            break;
        }
    }
    if (end != NULL) {
        /* We've got the delimter in the buffer */
        tocpy = end + 1 - file->bufiter; /* +1 includes the delimiter */
    } else if (file->bufiter < file->bufend) {
        /* No delimiter, but we return what we have. */
        tocpy = file->bufend - file->bufiter;
    } else {
        /* Nothign left at all */
        file->eof = 1;
        goto done;
    }
    /* we need to ensure that we still have enough room.
     * This happens as above */
    len += tocpy;
    while (len + 1 >= size) {
        size = kmroundupz(size + 1);
        buf = km_realloc_(buf, sizeof(*buf) * size, onerr, src, line);
        if (buf == NULL) {
            /* We bail out here, and *bufref is untouched. This means we
             * can check for errors, and free *bufref from the calling
             * function */
            return -2;
        }
    }
    nextbuf = buf + len - tocpy;
    memcpy(nextbuf, file->bufiter, tocpy);
    file->bufiter += tocpy;
    /* We don't bother updating nextbuf, as this was our final copy to buf */
    goto done;
done:
    /* restore/update referred values */
    *bufref = buf;
    *sizeref = size;
    if (len > 0) {
        /* We have something to return, so return its length */
        file->filepos += len;
        buf[len] = '\0';
        return len;
    } else if (file->eof) {
        return EOF;
    } else {
        /* Shouldn't reach here! */
        return -2;
    }
}

inline ssize_t
zfgetuntil (zfile_t *file, const int delim, char *dest, size_t maxlen)
{
    size_t len = 0;
    char *nextbuf = dest;
    size_t tocpy = 0;
    char *end = NULL;
    int ret = 0 ;
    if (dest == NULL || !zfile_ok(file) || maxlen < 1 || delim > 255) {
        /* EOF is normally == -1, so use -2 to differentiate them */
        return -2;
    }
    /* For detailed commentary, see zfgetuntil_realloc */
    /* Get out early if we're at EOF already */
    if (file->eof) {
        return EOF;
    }
    while ((end = strchr(file->bufiter, delim)) == NULL) {
        tocpy = file->bufend - file->bufiter;
        if (len + tocpy >= maxlen) {
            /* maxlen - 1 because we always leave space for \0 */
            tocpy -= len - maxlen - 1;
        }
        len += tocpy;
        memcpy(nextbuf, file->bufiter, tocpy);
        nextbuf += tocpy;
        file->bufiter += tocpy;
        dest[len] = '\0'; /* Null-terminate buf */
        ret = __zfile_fill_buffer(file);
        if (ret == 0) {
            return -2;
        } else if (ret == EOF) {
            break;
        }
    }
    if (end != NULL) {
        tocpy = end + 1 - file->bufiter; /* +1 includes the delimiter */
    } else if (file->bufiter < file->bufend) {
        tocpy = file->bufend - file->bufiter;
    } else {
        /* Nothign left at all */
        file->eof = 1;
        goto done;
    }
    len += tocpy;
    if (len + tocpy >= maxlen) {
        /* maxlen - 1 because we always leave space for \0 */
        tocpy -= len - maxlen - 1;
    }
    nextbuf = dest + len - tocpy;
    memcpy(nextbuf, file->bufiter, tocpy);
    file->bufiter += tocpy;
    goto done;
done:
    if (len > 0) {
        file->filepos += len;
        dest[len] = '\0';
        return len;
    } else if (file->eof) {
        return EOF;
    } else {
        return -2;
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
    ssize_t ln = 0;

    if (file == NULL || !str_ok(str)) {
        return -2; /* ERROR, not EOF */
    }
    ln = zfreadline_realloc(file, &(str->s), &(str->m));
    if (ln < 0) {
        str_nullify(str);
        return ln;
    } else {
        str->l = ln;
        return ln;
    }
}

inline void
zfprint_str (zfile_t *stream, const str_t *str)
{
    KM_ZWRITE(stream->fp, str->s, str->l);
}

inline int
zfgetc(zfile_t *file)
{
    if (!zfile_ok(file) || zfile_readable(file) == 0) {
        return -2;
    }
    if (file->eof) {
        return EOF;
    }
    return (file->bufiter++)[0];
}

const char *
zferror (zfile_t *file)
{
    int error = 0;
    const char *errstr = "";

    if (!zfile_ok(file)) {
        /* Never return NULL, or we'll SIGSEGV printf */
        return "BAD FILE";
    }
    errstr = gzerror(file->fp, &error);
    if (error == Z_ERRNO) {
        return strerror(errno);
    }
    return errstr;
}
