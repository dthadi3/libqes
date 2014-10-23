/*
 * ============================================================================
 *
 *       Filename:  qes_compat.h
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

#include "qes_config.h"

#ifndef HAVE_STRNDUP
char *strndup(const char *s, size_t *n);
#endif
