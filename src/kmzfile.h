/*
 * ============================================================================
 *
 *       Filename:  kmzfile.h
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

#ifndef KMZFILE_H
#define KMZFILE_H

#include <kmutil.h>
#include <kmstr.h>

typedef struct __zfile_t {
    KM_ZTYPE fp;
    char *path;
    off_t filepos;
    rwmode_t mode;
    char *buffer;
    char *bufiter;
    char *bufend;
    /* Is the fp at EOF, AND do we have nothing left to copy from the buffer */
    int eof  :1;
    /* Is the fp at EOF */
    int feof :1;
} zfile_t;

/* zfopen:
    Create a `zfile_t` and open `path` with mode `mode` and errorhandler
    `onerr`
 */
zfile_t *zfopen_ (const char *path, const char *mode, errhandler_t onerr,
        const char *file, int line);
#define    zfopen(pth, mod) \
    zfopen_(pth, mod, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define    zfopen_errnil(pth, mod) \
    zfopen_(pth, mod, errnil, __FILE__, __LINE__)
#define    zfopen_errprint(pth, mod) \
    zfopen_(pth, mod, errprint, __FILE__, __LINE__)
#define    zfopen_errprintexit(pth, mod) \
    zfopen_(pth, mod, errprintexit, __FILE__, __LINE__)


extern int zfile_guess_mode (const char *mode);
extern int zfpeek (zfile_t *file);
extern int zfgetc (zfile_t *file);
extern void zfrewind (zfile_t *file);

/*===  FUNCTION  ============================================================*
Name:           zfclose
Paramters:      zfile_t *file: file to close.
Description:    Closes the file pointer in ``file``, frees dynamically
                allocated members of ``file`` and sets ``file`` to NULL.
Returns:        void
 *===========================================================================*/
void zfclose_ (zfile_t *file);
#define zfclose(file) do {                      \
            zfclose_ (file);                    \
            file = NULL;                        \
        } while(0)

/*===  FUNCTION  ============================================================*
Name:           zfreadline
Paramters:      zfile_t *file: File to read
                char *dest: Destination buffer
                size_t maxlen: size of destination buffer
Description:    Reads at most ``maxlen - 1`` bytes of the next '\n' delimited
                line into ``dest``, and null- terminates ``dest``. The '\n' is
                copied, and therefore counts towards ``strlen()`` of ``dest``.
Returns:        ssize_t: EOF, -2 (error), or length of bytes copied, i.e.
                ``strlen(dest);``
 *===========================================================================*/
extern ssize_t zfreadline (zfile_t *file, char *dest, size_t maxlen);

/*===  FUNCTION  ============================================================*
Name:           zfgetuntil
Paramters:      zfile_t *file: File to read
                const int delim: Delimiter char.
Description:    Reads ``file`` into ``dest`` until ``delim`` is found or
                ``maxlen - `` bytes have been read. ``delim`` is copied into
                ``dest``! ``delim`` can be EOF for "read until EOF", or any
                other thing that fits in a ``char``.
Returns:        ssize_t: EOF, -2 (error) or size of data read.
 *===========================================================================*/
extern ssize_t zfgetuntil (zfile_t *file, const int delim, char *dest,
        size_t maxlen);

/*===  FUNCTION  ============================================================*
Name:           zfgetuntil_realloc
Paramters:      zfile *file: File to read.
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
extern ssize_t zfgetuntil_realloc_ (zfile_t *file, int delim, char **bufref,
        size_t *sizeref, errhandler_t onerr, const char *src, const int line);
#define zfgetuntil_realloc(fp, dlm, buf, sz) \
    zfgetuntil_realloc_(fp, dlm, buf, sz, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define zfgetuntil_realloc_errnil(fp, dlm, buf, sz) \
    zfgetuntil_realloc_(fp, dlm, buf, sz, errnil, __FILE__, __LINE__)
#define zfgetuntil_realloc_errprint(fp, dlm, buf, sz) \
    zfgetuntil_realloc_(fp, dlm, buf, sz, errprint, __FILE__, __LINE__)
#define zfgetuntil_realloc_errprintexit(fp, dlm, buf, sz) \
    zfgetuntil_realloc_(fp, dlm, buf, sz, errprintexit, __FILE__, __LINE__)

/*===  FUNCTION  ============================================================*
Name:           zfreadline_str
Paramters:      zfile_t *file: File to read.
                str_t *str: str_t object to read into.
Description:    Convenience wrapper around zfreadline_realloc, which reads a
                line into a str_t object, passing str->s to and str->m to
                zfreadline_realloc.
Returns:        ssize_t set to either the length of the line copied to the
                str_t, or one of -1 (EOF) or -2 (error).
* ===========================================================================*/
extern ssize_t zfreadline_str (zfile_t *file, str_t *str);

/* ===  FUNCTION  =============================================================
          Name: zfreadline_realloc
   Description: Read a line from `file` into a `char *` pointed to by `buf`.
                This function has the added benefit of `realloc`-ing `buf`
                to the next highest base-2 power, if we run out of space.
                If it is realloced, `(*size)` is updated to the new buffer
                size. DON'T USE ON STACK BUFFERS.
       Returns: ssize_t set to either the length of the line copied to `*buf`,
                or one of -1 (EOF) or -2 (error).
 * ==========================================================================*/
extern ssize_t zfreadline_realloc_ (zfile_t *file, char **buf, size_t *size,
              errhandler_t onerr, const char *src, const int line);
#define zfreadline_realloc(fp, buf, sz) \
    zfreadline_realloc_(fp, buf, sz, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define zfreadline_realloc_errnil(fp, buf, sz) \
    zfreadline_realloc_(fp, buf, sz, errnil, __FILE__, __LINE__)
#define zfreadline_realloc_errprint(fp, buf, sz) \
    zfreadline_realloc_(fp, buf, sz, errprint, __FILE__, __LINE__)
#define zfreadline_realloc_errprintexit(fp, buf, sz) \
    zfreadline_realloc_(fp, buf, sz, errprintexit, __FILE__, __LINE__)

extern void zfprint_str (zfile_t *stream, const str_t *str);
extern int zfile_ok(const zfile_t *zf);
extern int zfile_readable(zfile_t *zf);
extern int __zfile_fill_buffer (zfile_t *file);
#endif /* KMZFILE_H */
