/*
 * ============================================================================
 *
 *       Filename:  qes_error.h
 *
 *    Description:  Error handling functions
 *
 *        Created:  29/11/14 09:50:30
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#ifndef QES_ERROR_H
#define QES_ERROR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

struct qes_error {
    char *error;    /**< Textual description of error */
    char *file;     /**< File in which error occured */
    int status;     /**< Error code  */
    int flags;      /**< User-defined flags */
    int line;       /**< Line of file on which error occured */
};

typedef struct qes_error QesError;

/** qes_error_fprint
 *
 * \brief Print `err` to `stream`.
 *
 * @param   stream  FILE * to write to. Must be writable.
 * @param   err     Valid `struct qes_error`, containing error to print.
 * @return          Number of bytes written, or -1 on error.
 */
int qes_error_fprint(FILE               *stream,
                     struct qes_error   *err);

struct qes_error * _qes_error_new(int           status,
                                  const char   *error,
                                  int           flags,
                                  int           line,
                                  const char   *file);
#define qes_error_new(s, e, f) _qes_error_new(s, e, f, __LINE__, __FILE__)


void qes_error_propagate(struct qes_error **dest,
                         struct qes_error  *src);


struct qes_error * _qes_error_format(int            status,
                                     int            flags,
                                     int            line,
                                     const char    *file,
                                     const char    *error_fmt,
                                     ...);
#define qes_error_format(s, f, e, ...)                                      \
    _qes_error_format(s, f, __LINE__, __FILE__, e, __VA_ARGS__)

void _qes_error_free(struct qes_error   *err);
#define qes_error_free(err) ({ _qes_error_free(err); err = NULL; })

/* Helper macros */
#define qes_error_assert(fact, err) ({                                      \
        if (err != NULL) {                                                  \
            if (!(fact)) {                                                  \
                *err = _qes_error_new(-1, "Assertion Failed: " #fact, 0,    \
                                      __LINE__, __FILE__);                  \
            }                                                               \
        } })

#define qes_error_print_then_free(s, e) ({                                  \
        qes_error_fprint(s, e);                                             \
        qes_error_free(e);                                                  \
        })

#define qes_error_print_then_return(s, e, retval) qes_error_fprint(s, e);   \
        qes_error_free(e);                                                  \
        return retval  /* NO ';' */                                           \

#endif /* QES_ERROR_H */
