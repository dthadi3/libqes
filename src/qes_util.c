/*
 * ============================================================================
 *
 *       Filename:  qes_util.c
 *
 *    Description:  Wrappers around std library functions
 *
 *        Version:  1.0
 *        Created:  04/05/14 14:10:50
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_util.h"

/* Pull LIBQES_VERSION in from qes_config.h */
const char *libqes_version = LIBQES_VERSION;

/* Valid non-function to pass to libqes functions */
void
errnil (ERRFN_ARGS)
{
    (void) (msg);
    (void) (file);
    (void) (line);
}

/* Function to pass to libqes functions which prints out errors to stderr */
void
errprint (ERRFN_ARGS)
{
    char msg_fmt[1<<8] = "";
    va_list args;
    if (msg == NULL) {
        msg = "GENERIC ERROR WITH NO MESSAGE";
    }
    va_start (args, line);
    vsnprintf(msg_fmt, 1<<8, msg, args);
    va_end (args);
    fprintf(stderr, "[%s: %d]: %s\n", file, line, msg_fmt);
}

/* Function to pass to libqes functions which prints out errors to stderr and
   calls `exit(EXIT_FAILURE)` */
void
errprintexit (ERRFN_ARGS)
{
    char msg_fmt[1<<8] = "";
    va_list args;
    if (msg == NULL) {
        msg = "GENERIC ERROR WITH NO MESSAGE";
    }
    va_start (args, line);
    vsnprintf(msg_fmt, 1<<8, msg, args);
    va_end (args);
    fprintf(stderr, "[%s: %d]: %s\n", file, line, msg_fmt);
    QES_EXIT_FN(EXIT_FAILURE);
}

