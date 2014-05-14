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

typedef struct __zfile_t {
    KM_ZTYPE fp;
    char *path;
    int eof;
    off_t filepos;
    char *mode;
} zfile_t;

/* zfopen:
    Create a `zfile_t` and open `path` with mode `mode` and errorhandler
    `onerr`
 */
zfile_t *zfopen_ (const char *path, const char *mode, errhandler_t onerr,
        char *file, int line);
#define	zfopen(pth, mod) \
    zfopen_(pth, mod, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define	zfopen_errnil(pth, mod) \
    zfopen_(pth, mod, errnil, __FILE__, __LINE__)
#define	zfopen_errprint(pth, mod) \
    zfopen_(pth, mod, errprint, __FILE__, __LINE__)
#define	zfopen_errprintexit(pth, mod) \
    zfopen_(pth, mod, errprintexit, __FILE__, __LINE__)

/* zfclose:
    Destroy a `zfile_t` without leaking memory.
 */
void zfclose (zfile_t *file);

/* zfreadline:
    Read a line from `file` into a `char *` pointed to by `buf`, copying at
    most `maxlen` charachters into `*buf` INCLUDING the terminating '\0'.
 */
extern ssize_t zfreadline (zfile_t *file, char **dest, size_t maxlen);

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
#define km_readline_realloc(fp, buf, sz) \
    km_readline_realloc_(fp, buf, sz, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define km_readline_realloc_errnil(fp, buf, sz) \
    km_readline_realloc_(fp, buf, sz, errnil, __FILE__, __LINE__)
#define km_readline_realloc_errprint(fp, buf, sz) \
    km_readline_realloc_(fp, buf, sz, errprint, __FILE__, __LINE__)
#define km_readline_realloc_errprintexit(fp, buf, sz) \
    km_readline_realloc_(fp, buf, sz, errprintexit, __FILE__, __LINE__)

#endif /* KMZFILE_H */
