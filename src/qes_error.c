/*
 * ============================================================================
 *
 *       Filename:  qes_error.c
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
#include "qes_error.h"

struct qes_error *
_qes_error_new(int          status,
               const char  *error,
               int          flags,
               int          line,
               const char  *file)
{
    struct qes_error *err = NULL;

    if (error == NULL) {
        status = 1;
        error = "Invalid parameters to qes_error_new";
    }

    /* we use calloc directly, as our wrapper depends on this function */
    err = calloc(1, sizeof(*err));
    /* FIXME: This is very dodgy, however if there's not enough memory to
     * allocate the error handler, we're boned anyway. */
    if (err == NULL) {
        fprintf(stderr, "Couldn't allocate memory for an error hander");
        exit(EXIT_FAILURE);
    }

    /* fill the struct */
    err->status = status;
    err->error = strdup(error);
    err->flags = flags;
    err->line = line;
    if (file != NULL)
        err->file = strdup(file);
    return err;
}


struct qes_error *
_qes_error_format(int           status,
                  int           flags,
                  int           line,
                  const char   *file,
                  const char   *error_fmt,
                  ...)
{
    char buffer[256] = "";
    va_list args;

    va_start(args, error_fmt);
    vsnprintf(buffer, 256, error_fmt, args);
    va_end(args);

    return _qes_error_new(status, buffer, flags, line, file);
}

void
qes_error_propagate(struct qes_error    **dest,
                    struct qes_error    *src)
{
    if (src == NULL)
        return;

    if (dest == NULL) {
        qes_error_free(src);
        return;
    }

    if (*dest != NULL)
        qes_error_free(*dest);
    *dest = src;
}

void
_qes_error_free(struct qes_error   *err)
{
    if (err != NULL) {
        if (err->error != NULL) {
            free(err->error);
            err->error = NULL;
        }
        if (err->file != NULL) {
            free(err->file);
            err->file = NULL;
        }
        free(err);
    }
}

int
qes_error_fprint(FILE                *stream,
                 struct qes_error    *err)
{
    int bytes_printed = 0;
    int res = 0;

    res = fprintf(stream, "ERROR: %s", err->error);
    if (res > 0) {
        bytes_printed += res;
    } else {
        return res;
    }

    if (err->line > 0 && err->file != NULL) {
        res = fprintf(stream, " at line %d in %s", err->line, err->file);
        if (res > 0) {
            bytes_printed += res;
        } else {
            return res;
        }
    }

    res = fputc('\n', stream);
    if (res == '\n') {
        bytes_printed += 1;
    } else {
        return -1;
    }

    return bytes_printed;
}
