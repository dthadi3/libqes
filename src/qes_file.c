/*
 * ============================================================================
 *
 *       Filename:  qes_file.c
 *
 *    Description:  Compressed file IO
 *
 *        Version:  1.0
 *        Created:  09/05/14 12:20:57
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_file.h"

struct qes_file *
qes_file_open_ (const char *path, const char *mode, qes_errhandler_func onerr,
        const char *file, int line)
{
    struct qes_file *qf = NULL;

    /* Error out with NULL */
    if (path == NULL || mode == NULL || onerr == NULL || file == NULL) {
        return NULL;
    }

    /* create file struct */
    qf = qes_calloc(1, sizeof(*qf));
    /* Open file, handling any errors */
    qf->fp = QES_ZOPEN(path, mode);
    if (qf->fp == NULL) {
        (*onerr)("Opening file %s failed:\n%s\n", file, line,
                path, strerror(errno));
        qes_free(qf);
        return(NULL);
    }
    qf->mode = qes_file_guess_mode(mode);
    if (qf->mode == QES_READ_MODE_UNKNOWN) {
        QES_ZCLOSE(qf->fp);
        qes_free(qf);
        return NULL;
    }
    /* Use a larger than default IO buffer, speeds things up.
     * Using 2x our buffer len for no particular reason. */
#ifdef GZBUFFER_FOUND
    QES_ZBUFFER(qf->fp, (QES_FILEBUFFER_LEN) << 1);
#endif
    if (qf->mode == QES_READ_MODE_READ) {
#if defined(MEMALIGN_FOUND) && defined(GETPAGESIZE_FOUND)
        qf->buffer = aligned_alloc(getpagesize(),
                                   (QES_FILEBUFFER_LEN * sizeof(*qf->buffer)));
        if (qf->buffer == NULL) {
            QES_ZCLOSE(qf->fp);
            qes_free(qf);
            (*onerr)("Coudn't allocate aligned memory", file, line);
            return NULL;
        }
#else
        qf->buffer = qes_malloc_((QES_FILEBUFFER_LEN * sizeof(*qf->buffer)),
                onerr, file, line);
        if (qf->buffer == NULL) {
            QES_ZCLOSE(qf->fp);
            qes_free(qf);
            (*onerr)("Coudn't allocate buffer memory", file, line);
            return NULL;
        }
#endif
        qf->bufiter = qf->buffer;
        qf->buffer[0] = '\0';
        qf->bufend = qf->buffer;
        if (__qes_file_fill_buffer(qf) == 0) {
            qes_free(qf->buffer);
            qes_free(qf);
            return NULL;
        }
    }
    /* init struct fields */
    qf->eof = 0;
    qf->filepos = 0;
    qf->path = strndup(path, QES_MAX_FN_LEN);
    return(qf);
}

int
qes_file_guess_mode (const char *mode)
{
    if (mode[0] == 'r') {
        return QES_READ_MODE_READ;
    } else if(mode[0] == 'w' || mode[0] == 'a') {
        return QES_READ_MODE_WRITE;
    }
    return QES_READ_MODE_UNKNOWN;
}

void
qes_file_rewind (struct qes_file *file)
{
    if (qes_file_ok(file)) {
        QES_ZSEEK(file->fp, 0, SEEK_SET);
        file->filepos = 0;
        file->eof = 0;
        file->feof = 0;
        file->bufiter = file->buffer;
        file->bufend = file->buffer;
    }
}

void
qes_file_close_ (struct qes_file *file)
{
    if (file != NULL) {
        if (file->fp != NULL) {
            QES_ZCLOSE(file->fp);
        }
        qes_free(file->path);
        qes_free(file->buffer);
        file->bufiter = NULL;
        file->bufend = NULL;
        qes_free(file);
    }
}


const char *
qes_file_error (struct qes_file *file)
{
    int error = 0;
    const char *errstr = "";

    if (!qes_file_ok(file)) {
        /* Never return NULL, or we'll SIGSEGV printf */
        return "BAD FILE";
    }
#ifdef ZLIB_FOUND
    errstr = gzerror(file->fp, &error);
    if (error == Z_ERRNO) {
        return strerror(errno);
    }
#else
    error = ferror(file->fp);
    if (error != 0) {
        errstr = strerror(errno);
        clearerr(file->fp);
    }
#endif
    return errstr;
}
