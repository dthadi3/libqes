/*
 * ============================================================================
 *
 *       Filename:  qes_compat.h
 *
 *    Description:  Compatibility helpers for cross-platformness
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_config.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>


#ifndef STRNDUP_FOUND
char *strndup(const char *s, size_t n);
#endif

#ifndef VASPRINTF_FOUND
int vasprintf(char **ret, const char *format, va_list args);
#endif

#ifndef ASPRINTF_FOUND
int asprintf(char **ret, const char *format, ...);
#endif
