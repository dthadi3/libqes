/*
 * ============================================================================
 *
 *       Filename:  qes_compat.c
 *
 *    Description:  Compatibility helpers for cross-platformness
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_compat.h"
#include <stdlib.h>


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
