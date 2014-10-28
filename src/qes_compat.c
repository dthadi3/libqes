/*
 * ============================================================================
 *
 *       Filename:  qes_compat.c
 *
 *    Description:  Compatibility helpers for cross-platformness
 *
 *        Created:  24/10/14 08:59:24
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#include <stdlib.h>
#include "qes_compat.h"

#ifndef STRNDUP_FOUND
char *
strndup(const char *s, size_t n)
{
    char *dest = malloc(n+1);
    if (dest == NULL) return NULL;
    strncpy(dest, s, n);
    dest[n] = '\0';
    return dest;
}
#endif
