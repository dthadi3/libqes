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
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
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

#include <kmbase.h>

/*  CONSTANTS  */
#define KMFILE_BUFFER_LEN (size_t)(1<<20)   /* 1Mb */

typedef struct {
    __FP_TYPE file;
    char *path;
    int eof;
    int err;
    int filepos;
    char mode;
    /* buffer <= bufferiter < bufferend <= buffer + KMFILE_BUFFER_LEN - 1 */
    char *buffer;
    char *bufferiter;
    char *bufferend;
} kmfile;

kmfile *create_kmfile (const char * filename);
void destroy_kmfile (kmfile *file);
extern size_t __readline_kmfile_keep (kmfile *file, char **dest, size_t maxlen);
extern void __kmfile_fill_buffer (kmfile *file);
size_t readline_kmfile (kmfile *file, char **dest, size_t maxlen);
size_t hint_line_length_kmfile (kmfile *file);
char peek_ahead_kmfile (kmfile *file);

#endif /* KMFILE_H */
