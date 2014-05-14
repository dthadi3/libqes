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
 *   Create a `zfile_t` and open `path` with mode `mode` and errorhandler
 *   `onerr`
 */
zfile_t *zfopen_ (const char *path, const char *mode, errhandler_t onerr,
        char *file, int line);
#define	zfopen(pth, mod, oe) \
    zfopen_(pth, mod, oe, __FILE__, __LINE__)

/* zfclose:
 *   Destroy a `zfile_t` without leaking memory.
 */
void zfclose (zfile_t *file);

extern ssize_t zfreadline (zfile_t *file, char **dest, size_t maxlen);
extern ssize_t zfreadline_realloc_ (zfile_t *file, char **buf, size_t *size,
              errhandler_t onerr, const char *src, const int line);
#define km_readline_realloc(fp, buf, sz, fn) \
    km_readline_realloc_(fp, buf, sz, fn, __FILE__, __LINE__)

#endif /* KMZFILE_H */
