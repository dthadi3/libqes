/*
 * ============================================================================
 *
 *       Filename:  qes_file.c
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

#include "qes_file.h"

struct qes_file *
qes_file_open_ (const char *path, const char *mode, qes_errhandler_func onerr,
        const char *file, int line)
{
    /* create file struct */
    struct qes_file *qf = NULL;
    if (path == NULL || mode == NULL) return NULL;
    qf = qes_calloc(1, sizeof(*qf));
    /* Open file, handling any errors */
    qf->fp = QES_ZOPEN(path, mode);
    if (qf->fp == NULL) {
        (*onerr)("Opening file %s failed:\n%s\n", file, line,
                path, strerror(errno));
        qes_free(qf);
        return(NULL);
    }
    qf->mode = qes_file_guess_mode(mode);
    if (qf->mode == RW_UNKNOWN) {
        QES_ZCLOSE(qf->fp);
        qes_free(qf);
        return NULL;
    }
    /* Use a larger than default IO buffer, speeds things up.
     * Using 2x our buffer len for no particular reason. */
    QES_ZBUFFER(qf->fp, (QES_FILEBUFFER_LEN) << 1);
    if (qf->mode == RW_READ || qf-> mode == RW_READWRITE) {
#ifdef HAVE_POSIX_MEMALIGN
        posix_memalign((void *)&(qf->buffer), getpagesize(),
                (QES_FILEBUFFER_LEN * sizeof(*qf->buffer)));
        if (qf->buffer == NULL) {
            QES_ZCLOSE(qf->fp);
            qes_free(qf);
            (*onerr)("Coudn't allocate aligned memory", file, line);
            return NULL;
        }
#else
        qf->buffer = qes_malloc_((QES_FILEBUFFER_LEN * sizeof(*qf->buffer)),
                onerr, file, line);
        if (qf->buffer == NULL) {
            QES_ZCLOSE(qf->fp);
            qes_free(qf);
            (*onerr)("Coudn't allocate aligned memory", file, line);
            return NULL;
        }
#endif
        qf->bufiter = qf->buffer;
        qf->buffer[0] = '\0';
        qf->bufend = qf->buffer;
        if (__qes_file_fill_buffer(qf) == 0) {
            qes_free(qf->buffer);
            qes_free(qf);
            return NULL;
        }
    }
    /* init struct fields */
    qf->eof = 0;
    qf->filepos = 0;
    qf->path = strndup(path, QES_MAX_FN_LEN);
    return(qf);
}

inline int
qes_file_guess_mode (const char *mode)
{
    if (mode[0] == 'r') {
        return RW_READ;
    } else if(mode[0] == 'w' || mode[0] == 'a') {
        return RW_WRITE;
    }
    return RW_UNKNOWN;
}

inline void
qes_file_rewind (struct qes_file *file)
{
    if (qes_file_ok(file)) {
        QES_ZSEEK(file->fp, 0, SEEK_SET);
        file->filepos = 0;
        file->eof = 0;
        file->feof = 0;
        file->bufiter = file->buffer;
        file->bufend = file->buffer;
    }
}

void
qes_file_close_ (struct qes_file *file)
{
    if (file != NULL) {
        if (file->fp != NULL) {
            QES_ZCLOSE(file->fp);
        }
        qes_free(file->path);
        qes_free(file->buffer);
        file->bufiter = NULL;
        file->bufend = NULL;
        qes_free(file);
    }
}

inline ssize_t
qes_file_readline_realloc_ (struct qes_file *file, char **buf, size_t *size,
        qes_errhandler_func onerr, const char *src, const int line)
{
    return qes_file_getuntil_realloc_(file, '\n', buf, size, onerr, src, line);
}

inline ssize_t
qes_file_getuntil_realloc_ (struct qes_file *file, int delim, char **bufref, size_t *sizeref,
        qes_errhandler_func onerr, const char *src, const int line)
{
    size_t len = 0;
    size_t tocpy = 0;
    char *buf = NULL;
    char *nextbuf = NULL;
    char *end = NULL;
    size_t size = 0;
    int ret = 0;
    if (bufref == NULL || !qes_file_ok(file) || sizeref == NULL) {
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
        buf = qes_malloc_(__INIT_LINE_LEN * sizeof(*buf), onerr, src, line);
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
            size = qes_roundupz(size);
            buf = qes_realloc_(buf, sizeof(*buf) * size, onerr, src, line);
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
        ret = __qes_file_fill_buffer(file);
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
        size = qes_roundupz(size + 1);
        buf = qes_realloc_(buf, sizeof(*buf) * size, onerr, src, line);
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
qes_file_getuntil (struct qes_file *file, const int delim, char *dest, size_t maxlen)
{
    size_t len = 0;
    char *nextbuf = dest;
    size_t tocpy = 0;
    char *end = NULL;
    int ret = 0 ;
    if (dest == NULL || !qes_file_ok(file) || maxlen < 1 || delim > 255) {
        /* EOF is normally == -1, so use -2 to differentiate them */
        return -2;
    }
    /* For detailed commentary, see qes_file_getuntil_realloc */
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
        ret = __qes_file_fill_buffer(file);
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
qes_file_readline (struct qes_file *file, char *dest, size_t maxlen)
{
    return qes_file_getuntil(file, '\n', dest, maxlen);
}

inline ssize_t
qes_file_readline_str (struct qes_file *file, struct qes_str *str)
{
    ssize_t ln = 0;

    if (file == NULL || !qes_str_ok(str)) {
        return -2; /* ERROR, not EOF */
    }
    ln = qes_file_readline_realloc(file, &(str->s), &(str->m));
    if (ln < 0) {
        qes_str_nullify(str);
        return ln;
    } else {
        str->l = ln;
        return ln;
    }
}

inline void
qes_file_print_str (struct qes_file *stream, const struct qes_str *str)
{
    QES_ZWRITE(stream->fp, str->s, str->l);
}

inline int
qes_file_getc(struct qes_file *file)
{
    if (!qes_file_ok(file) || qes_file_readable(file) == 0) {
        return -2;
    }
    if (file->eof) {
        return EOF;
    }
    return (file->bufiter++)[0];
}

const char *
qes_file_error (struct qes_file *file)
{
    int error = 0;
    const char *errstr = "";

    if (!qes_file_ok(file)) {
        /* Never return NULL, or we'll SIGSEGV printf */
        return "BAD FILE";
    }
    errstr = gzerror(file->fp, &error);
    if (error == Z_ERRNO) {
        return strerror(errno);
    }
    return errstr;
}
