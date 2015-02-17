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

#ifndef VASPRINTF_FOUND
int vasprintf(char **ret, const char *format, va_list args)
{
    va_list copy;
    int count;
    va_copy(copy, args);

    *ret = NULL;

    count = vsnprintf(NULL, 0, format, args);
    if (count >= 0) {
        char *buffer = malloc(count + 1);
        if (buffer == NULL) {
            count = -1;
        } else if ((count = vsnprintf(buffer, count + 1, format, copy)) < 0) {
            free(buffer);
        }
        else {
            *ret = buffer;
        }
    }
    va_end(copy);

    return count;
}
#endif

#ifndef ASPRINTF_FOUND
int asprintf(char **ret, const char *format, ...)
{
    va_list args;
    int count;

    va_start(args, format);
    count = vasprintf(ret, format, args);
    va_end(args);
    return(count);
}
#endif
