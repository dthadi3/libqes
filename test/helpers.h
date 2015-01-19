/*
 * ============================================================================
 *
 *       Filename:  helpers.h
 *
 *    Description:  Helpers for tests
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#ifndef _WIN32
# include <unistd.h>
#endif

#include <qes_libgnu.h>

extern char *data_prefix;

char *find_data_file(const char * filepath);
char *get_writable_file(void);
void clean_writable_file(char *filepath);
char *crc32_file(const char *filepath);
int filecmp(const char *file1, const char *file2);

#endif /* HELPERS_H */
