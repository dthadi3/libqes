/*
 * ============================================================================
 *
 *       Filename:  qes_libgnu.c
 *    Description:  Functions required from gnulib
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_libgnu.h"

#ifndef ZLIB_FOUND
# include "crc.c"
#endif
