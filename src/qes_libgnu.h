/*
 * ============================================================================
 *
 *       Filename:  qes_libgnu.h
 *    Description:  Functions required from gnulib
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef QES_LIBGNU_H
#define QES_LIBGNU_H

#include "qes_config.h"

/* This file and qes_libgnu.c are designed to allow us to keep the sources of
 * the gnulib functions intact and in their original seperate form. */

#ifndef ZLIB_FOUND
# include "crc.h"
#else
# include <zlib.h>
/* Cast is to avoid a difference in signed-ness in the two implementations. */
# define crc32_update(c, b, l) crc32(c, (const unsigned char *)b, l)
#endif

#endif /* QES_LIBGNU_H */
