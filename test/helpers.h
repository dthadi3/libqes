/*
 * ============================================================================
 *
 *       Filename:  helpers.h
 *
 *    Description:  Helpers for tests
 *
 *        Version:  1.0
 *        Created:  29/05/14 16:22:23
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include <zlib.h>

extern char *data_prefix;

char *find_data_file(const char * filepath);
char *get_writable_file();
void clean_writable_file(char *filepath);

#endif /* HELPERS_H */
