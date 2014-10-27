/*
 * ============================================================================
 *
 *       Filename:  qes_file.h
 *
 *    Description:  Compressed file IO
 *
 *        Version:  1.0
 *        Created:  09/05/14 12:25:07
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef QES_FILE_H
#define QES_FILE_H

#include <qes_util.h>
#include <qes_str.h>

enum qes_read_mode {
    QES_READ_MODE_UNKNOWN,
    QES_READ_MODE_READ,
    QES_READ_MODE_WRITE,
    QES_READ_MODE_READWRITE,
};

struct qes_file {
    QES_ZTYPE fp;
    char *path;
    off_t filepos;
    enum qes_read_mode mode;
    char *buffer;
    char *bufiter;
    char *bufend;
    /* Is the fp at EOF, AND do we have nothing left to copy from the buffer */
    int eof  :1;
    /* Is the fp at EOF */
    int feof :1;
};

/* qes_file_open:
    Create a `struct qes_file` and open `path` with mode `mode` and errorhandler
    `onerr`
 */
struct qes_file *qes_file_open_ (const char *path, const char *mode,
                                 qes_errhandler_func onerr, const char *file,
                                 int line);
#define    qes_file_open(pth, mod) \
    qes_file_open_(pth, mod, QES_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define    qes_file_open_errnil(pth, mod) \
    qes_file_open_(pth, mod, errnil, __FILE__, __LINE__)
#define    qes_file_open_errprint(pth, mod) \
    qes_file_open_(pth, mod, errprint, __FILE__, __LINE__)
#define    qes_file_open_errprintexit(pth, mod) \
    qes_file_open_(pth, mod, errprintexit, __FILE__, __LINE__)


/*===  FUNCTION  ============================================================*
Name:           qes_file_close
Paramters:      struct qes_file *file: file to close.
Description:    Closes the file pointer in ``file``, frees dynamically
                allocated members of ``file`` and sets ``file`` to NULL.
Returns:        void
 *===========================================================================*/
void qes_file_close_ (struct qes_file *file);
#define qes_file_close(file) do {                      \
            qes_file_close_ (file);                    \
            file = NULL;                        \
        } while(0)

const char * qes_file_error (struct qes_file *file);
int qes_file_guess_mode (const char *mode);
void qes_file_rewind (struct qes_file *file);

/* INLINE FUNCTIONS */

static inline int
qes_file_ok(const struct qes_file *qf)
{
    /* qes_file_ok just check we won't dereference NULLs, so we check pointer
     * NULLness for all pointers we care about in current modes. Which, unless
     * we're Write-only, is all of them */
    return  qf != NULL && \
            qf->fp != NULL && \
            (qf->mode == QES_READ_MODE_WRITE || \
                (qf->bufiter != NULL && \
                 qf->buffer != NULL)
            );
}

static inline int
__qes_file_fill_buffer (struct qes_file *file)
{
    ssize_t res = 0;
    if (!qes_file_ok(file)) {
        return 0;
    }
    if (file->feof || file->eof) {
        file->eof = 1;
        return EOF;
    }
    res = QES_ZREAD(file->fp, file->buffer, (QES_FILEBUFFER_LEN) - 1);
    if (res < 0) {
        /* Errored */
        return 0;
    } else if (res == 0) {
        /* At both buffer & file EOF */
        file->eof = 1;
        file->feof = 1;
        return EOF;
    } else if (res < (QES_FILEBUFFER_LEN) - 1) {
        /* At file EOF */
        file->feof = 1;
    }
    file->bufiter = file->buffer;
    file->bufend = file->buffer + res;
    file->bufend[0] = '\0';
    return 1;
}

static inline int
qes_file_readable(struct qes_file *file)
{
    /* Here we check that reads won't fail. We refil if we need to. */
    /* Can we possibly read from this file? */
    if (!qes_file_ok(file) || file->mode == QES_READ_MODE_UNKNOWN || \
            file->mode == QES_READ_MODE_WRITE || file->eof) {
        return 0;
    }
    /* We can read from buffer */
    if (file->bufiter < file->bufend && file->bufiter[0] != '\0') {
        return 1;
    }
    /* Buffer needs a refil */
    if (__qes_file_fill_buffer(file) != 0) {
        /* we either successfully refilled, or are at EOF */
        return file->eof ? EOF : 1;
    } else {
        /* No, we can't read */
        return 0;
    }
}

static inline int
qes_file_writable(struct qes_file *file)
{
    /* Here we check that reads won't fail. We refil if we need to. */
    /* Can we possibly read from this file? */
    if (!qes_file_ok(file) || file->mode == QES_READ_MODE_UNKNOWN || \
            file->mode == QES_READ_MODE_READ) {
        return 0;
    }
    /* TODO: be more rigorous here */
    return 1;
}

static inline int
qes_file_peek(struct qes_file *file)
{
    if (!qes_file_ok(file) || qes_file_readable(file) == 0) {
        return -2;
    } else if (file->eof) {
        return EOF;
    }
    return file->bufiter[0];
}

static inline void
qes_file_print_str (struct qes_file *stream, const struct qes_str *str)
{
    QES_ZWRITE(stream->fp, str->str, str->len);
}

static inline int
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

static inline ssize_t
qes_file_puts(struct qes_file *file, const char *str)
{
    if (!qes_file_ok(file) || !qes_file_writable(file)) {
        return -2;
    }
    return QES_ZFPUTS(file->fp, str);
}

static inline ssize_t
qes_file_putc(struct qes_file *file, const char chr)
{
    int res = 0;
    if (!qes_file_ok(file) || !qes_file_writable(file)) {
        return -2;
    }
    res = QES_ZFPUTC(file->fp, chr);
    if (res != chr) {
        return -1;
    }
    return 1;
}

/*===  FUNCTION  ============================================================*
Name:           qes_file_getuntil_realloc
Paramters:      qes_file *file: File to read.
                int delim: Delimiter char.
                char **bufref: reference to a `char *` containing the buffer.
                    Must not refer to a ``char[]`` that cannot be resized with
                    ``realloc``.
                size *sizeref: Reference to a variable tracking the allocated
                    size of the ``char *`` referred to by ``bufref``.
Description:    Read a string from `file` into a
                `char *` pointed to by
                `bufref` up to and inclding the character ``delim``. This
                function has the added benefit of `realloc`-ing `*bufref` to
                the next highest base-2 power, if we run out of space.  If it
                is realloced, `(*sizeref)` is updated to the new buffer size.
Returns:        ssize_t set to either the length of the line copied to
                `*bufref`, or one of -1 (EOF) or -2 (error).
*============================================================================*/
static inline ssize_t
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
#define qes_file_getuntil_realloc(fp, dlm, buf, sz)                         \
    qes_file_getuntil_realloc_(fp, dlm, buf, sz, QES_DEFAULT_ERR_FN,        \
                               __FILE__, __LINE__)
#define qes_file_getuntil_realloc_errnil(fp, dlm, buf, sz)                  \
    qes_file_getuntil_realloc_(fp, dlm, buf, sz, errnil, __FILE__, __LINE__)
#define qes_file_getuntil_realloc_errprint(fp, dlm, buf, sz)                \
    qes_file_getuntil_realloc_(fp, dlm, buf, sz, errprint, __FILE__, __LINE__)
#define qes_file_getuntil_realloc_errprintexit(fp, dlm, buf, sz)            \
    qes_file_getuntil_realloc_(fp, dlm, buf, sz, errprintexit, __FILE__,    \
                               __LINE__)


/* ===  FUNCTION  =============================================================
          Name: qes_file_readline_realloc
   Description: Read a line from `file` into a `char *` pointed to by `buf`.
                This function has the added benefit of `realloc`-ing `buf`
                to the next highest base-2 power, if we run out of space.
                If it is realloced, `(*size)` is updated to the new buffer
                size. DON'T USE ON STACK BUFFERS.
       Returns: ssize_t set to either the length of the line copied to `*buf`,
                or one of -1 (EOF) or -2 (error).
 * ==========================================================================*/
static inline ssize_t
qes_file_readline_realloc_ (struct qes_file *file, char **buf, size_t *size,
        qes_errhandler_func onerr, const char *src, const int line)
{
    return qes_file_getuntil_realloc_(file, '\n', buf, size, onerr, src, line);
}
#define qes_file_readline_realloc(fp, buf, sz) \
    qes_file_readline_realloc_(fp, buf, sz, QES_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define qes_file_readline_realloc_errnil(fp, buf, sz) \
    qes_file_readline_realloc_(fp, buf, sz, errnil, __FILE__, __LINE__)
#define qes_file_readline_realloc_errprint(fp, buf, sz) \
    qes_file_readline_realloc_(fp, buf, sz, errprint, __FILE__, __LINE__)
#define qes_file_readline_realloc_errprintexit(fp, buf, sz) \
    qes_file_readline_realloc_(fp, buf, sz, errprintexit, __FILE__, __LINE__)


/*===  FUNCTION  ============================================================*
Name:           qes_file_getuntil
Paramters:      struct qes_file *file: File to read
                const int delim: Delimiter char.
Description:    Reads ``file`` into ``dest`` until ``delim`` is found or
                ``maxlen - `` bytes have been read. ``delim`` is copied into
                ``dest``! ``delim`` can be EOF for "read until EOF", or any
                other thing that fits in a ``char``.
Returns:        ssize_t: EOF, -2 (error) or size of data read.
 *===========================================================================*/
static inline ssize_t
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

/*===  FUNCTION  ============================================================*
Name:           qes_file_readline
Paramters:      struct qes_file *file: File to read
                char *dest: Destination buffer
                size_t maxlen: size of destination buffer
Description:    Reads at most ``maxlen - 1`` bytes of the next '\n' delimited
                line into ``dest``, and null- terminates ``dest``. The '\n' is
                copied, and therefore counts towards ``strlen()`` of ``dest``.
Returns:        ssize_t: EOF, -2 (error), or length of bytes copied, i.e.
                ``strlen(dest);``
 *===========================================================================*/
static inline ssize_t
qes_file_readline (struct qes_file *file, char *dest, size_t maxlen)
{
    return qes_file_getuntil(file, '\n', dest, maxlen);
}

/*===  FUNCTION  ============================================================*
Name:           qes_file_readline_str
Paramters:      struct qes_file *file: File to read.
                struct qes_str *str: struct qes_str object to read into.
Description:    Convenience wrapper around qes_file_readline_realloc, which reads a
                line into a struct qes_str object, passing str->str to and str->capacity to
                qes_file_readline_realloc.
Returns:        ssize_t set to either the length of the line copied to the
                struct qes_str, or one of -1 (EOF) or -2 (error).
* ===========================================================================*/
static inline ssize_t
qes_file_readline_str (struct qes_file *file, struct qes_str *str)
{
    ssize_t len = 0;

    if (file == NULL || !qes_str_ok(str)) {
        return -2; /* ERROR, not EOF */
    }
    len = qes_file_readline_realloc(file, &(str->str), &(str->capacity));
    if (len < 0) {
        qes_str_nullify(str);
        return len;
    }
    str->len = len;
    return len;
}

#endif /* QES_FILE_H */
