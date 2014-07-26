/*
 * ============================================================================
 *
 *       Filename:  kmutil.c
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

#include "kmutil.h"

/* Pull KMLIB_VERSION in from CMAKE */
const char *kmlib_version = KMLIB_VERSION;

/* Valid non-function to pass to libkdm functions */
void
errnil (ERRFN_ARGS)
{
    (void) (msg);
    (void) (file);
    (void) (line);
}

/* Function to pass to libkdm functions which prints out errors to stderr */
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

/* Function to pass to libkdm functions which prints out errors to stderr and
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
    KM_EXIT_FN(EXIT_FAILURE);
}

/*
 * Memory allocation/deallocation
 */
inline void *
km_calloc_ (size_t n, size_t size, errhandler_t onerr, const char *file,
        int line)
{
    void *ret = calloc(n, size);
    if (ret == NULL) {
        (*onerr)("calloc returned NULL -- Out of memory", file, line);
        return NULL;
    } else {
        return ret;
    }
}

inline void *
km_malloc_ (size_t size, errhandler_t onerr, const char *file, int line)
{
    void *ret = malloc(size);
    if (ret == NULL) {
        (*onerr)("malloc returned NULL -- Out of memory", file, line);
        return NULL;
    } else {
        return ret;
    }
}

inline void *
km_realloc_ (void *data, size_t size, errhandler_t onerr, const char *file,
        int line)
{
    void *ret = realloc(data, size);
    if (ret == NULL) {
        (*onerr)("realloc returned NULL -- Out of memory", file, line);
        return NULL;
    } else {
        return ret;
    }
}
