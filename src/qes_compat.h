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


#ifndef STRNDUP_FOUND
char *strndup(const char *s, size_t n);
#endif
