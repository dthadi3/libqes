/*
 * ============================================================================
 *
 *       Filename:  kmfile.h
 *
 *    Description:  Generic text file IO for sequence data
 *
 *        Version:  1.0
 *        Created:  02/12/13 17:21:50
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#ifndef KMFILE_H
#define KMFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <zlib.h>

#include <kmutil.h>
#include <kmstr.h>

/*  CONSTANTS  */
#define KMFILE_BUFFER_LEN (size_t)(1<<20)   /* 1Mb */

typedef struct __file_t {
    FILE* fp;
    char *path;
    int eof;
    int err;
    int filepos;
    char *mode;
    /* buffer <= bufferiter < bufferend <= buffer + KMFILE_BUFFER_LEN - 1 */
    char *buffer;
    char *bufferiter;
    char *bufferend;
} file_t;


file_t *create_file (const char * filename, const char *mode);
void destroy_file (file_t *file);
extern size_t __readline_file_keep (file_t *file, char **dest, size_t maxlen);
extern void __file_fill_buffer (file_t *file);
size_t readline_file (file_t *file, char **dest, size_t maxlen);
extern size_t hint_line_length_file (file_t *file);
extern char peek_ahead_file (file_t *file);

inline int
zfile_guess_mode (const char *mode)
{
    if (strncmp(mode, "r+", 2) == 0) {
        return RW_READWRITE;
    } else if(strncmp(mode, "a+") == 0) {
        return RW_READWRITE;
    }
    }
}
/*
 * IO routines
 */

extern file_t *
km_fopen (const char *path, const char *mode,
          errhandler_t onerr, char *file, int line)
{
    char magic[4];
    FILE *tmp = NULL;
    file_t *res = km_calloc(1, sizeof(*res), &errprintexit);
    tmp = fopen(path, "rb");
    if (tmp != NULL) {
        fread(magic, 4, 1, tmp);
    }

    if (tmp == NULL) {
        (*onerr)(strerror(errno), file, line);
    }
}

#define km_readline_realloc(buf, fp, sz, fn) \
    km_readline_realloc_(buf, fp, sz, fn, __FILE__, __LINE__)

#endif /* KMFILE_H */
