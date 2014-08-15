/*
 ============================================================================
 *
 *       Filename:  qes_util.h
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

#ifndef QES_UTIL_H
#define QES_UTIL_H

/* #####   HEADER FILE INCLUDES   ########################################## */
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*
 * Cross-platform bollocks. Thanks windows.
 */

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#define QES_PATHSEP "\\"
#else
#define QES_PATHSEP "/"
#include <unistd.h>
#endif



/*
 * Misc constants
 */

extern const char *libqes_version;

#define QES_MAX_FN_LEN (1<<16)
/* Size of buffers for file IO */
#define    QES_FILEBUFFER_LEN (1<<20)
/* Starting point for allocing a char pointer. Set to slightly larger than the
   standard size of whatever you're reading in. */
#define    __INIT_LINE_LEN (128)

/*
 * Macro helpers from tor
 */

/* Expands to a syntactically valid empty statement.  */
#define STMT_NIL (void)0

/* Expands to a syntactically valid empty statement, explicitly (void)ing its
 * argument. */
#define STMT_VOID(a) while (0) { (void)(a); }

/* STMT_BEGIN and STMT_END are used to wrap blocks inside macros so that
 * the macro can be used as if it were a single C statement. */
#ifdef __GNUC__
    #define STMT_BEGIN (void) ({
    #define STMT_END })
#elif defined(sun) || defined(__sun__)
    #define STMT_BEGIN if (1) {
    #define STMT_END } else STMT_NIL
#else
    #define STMT_BEGIN do {
    #define STMT_END } while (0)
#endif

/* This can be helpful in some macros, particularly with #pragma */
#ifndef STRINGIFY
    #define STRINGIFY(a) #a
#endif

/*
 * General helper macros
 */
#define qes_likely(x)      __builtin_expect(!!(x), 1)
#define qes_unlikely(x)    __builtin_expect(!!(x), 0)


/*
 * Error handling functions
 */

/* use the stdlib exit function by default, during testing we can #define this
   to some kind of error handler if we need to. */
#ifndef  QES_EXIT_FN
    #define  QES_EXIT_FN exit
#endif


/* By default, we use this error handler. At compile or include time, we can
   chose another more appropriate one if we need to. */
#ifndef  QES_DEFAULT_ERR_FN
    #define  QES_DEFAULT_ERR_FN errprintexit
#endif


#define ERRFN_ARGS const char *msg,  const char *file, int line, ...
void errnil(ERRFN_ARGS);
void errprint (ERRFN_ARGS);
void errprintexit (ERRFN_ARGS)  __attribute__ ((noreturn));
typedef void (*qes_errhandler_func) (const char*, const char *, int, ...);

/* qes_roundupz:
 *   Round up a `size_t` to the next highest power of two.
 */
/* Flogged from http://stackoverflow.com/a/1322548 and
   http://graphics.stanford.edu/~seander/bithacks.html, and kseq.h */
/* Round a 32-bit int up to nearest base-2 number */
static inline size_t
qes_roundupz (size_t sz)
{
    /* Decrement v only if v is not already a power of 2 */
    /* I.e, roundup things already a power of 2 */
    if (qes_unlikely((sz & (sz - 1)) != 0)) sz--;
    /* mask all bits below MSB to 1 */
    sz |= sz>>1;
    sz |= sz>>2;
    sz |= sz>>4;
    sz |= sz>>8;
    sz |= sz>>16;
#if UINTPTR_MAX == 0xffffffffffffffff /* 64-bit system */
    sz |= sz>>32;
#endif
    return sz + 1;
}

static inline uint32_t
qes_roundup32 (uint32_t u32)
{
    /* Roundup things already a power of 2 */
    if ((u32 & (u32 - 1)) != 0) u32--;
    /* mask all bits below MSB to 1 */
    u32 |= u32>>1;
    u32 |= u32>>2;
    u32 |= u32>>4;
    u32 |= u32>>8;
    u32 |= u32>>16;
    return u32 + 1;
}

static inline uint64_t
qes_roundup64 (uint64_t u64)
{
    /* Roundup things already a power of 2 */
    if ((u64 & (u64 - 1)) != 0) u64--;
    /* mask all bits below MSB to 1 */
    u64 |= u64>>1;
    u64 |= u64>>2;
    u64 |= u64>>4;
    u64 |= u64>>8;
    u64 |= u64>>16;
    u64 |= u64>>32;
    return u64 + 1;
}




/* IO helpers */

/* Definitions to make changing fp type easy */
#ifdef HAVE_ZLIB
#   include <zlib.h>
#   define    QES_ZTYPE gzFile
#   define    QES_ZOPEN gzopen
#   define    QES_ZDOPEN gzdopen
#   define    QES_ZCLOSE gzclose
#   define    QES_ZREAD gzread
#   define    QES_ZWRITE gzwrite
#   define    QES_ZFLUSH gzflush
#   define    QES_ZFPRINTF gzprintf
#   define    QES_ZFPUTS gzputs
#   define    QES_ZFPUTC gzputc
#   define    QES_ZFGETS gzgets
#   define    QES_ZFGETC gzgetc
#   define    QES_ZFUNGETC gzungetc
#   define    QES_ZERR gzerror
#   define    QES_ZEOF gzeof
#   define    QES_ZBUFFER gzbuffer
#   define    QES_ZSEEK gzseek
#   define    QES_ZTELL gztell
#   define    QES_ZREWIND gzrewind
#else
#   define    QES_ZTYPE FILE*
#   define    QES_ZOPEN fopen
#   define    QES_ZCLOSE fclose
#   define    QES_ZDOPEN fdopen
#   define    QES_ZCLOSE fclose
#   define    QES_ZREAD fread
#   define    QES_ZWRITE fwrite
#   define    QES_ZFLUSH fflush
#   define    QES_ZFPRINTF fprintf
#   define    QES_ZFPUTS fputs
#   define    QES_ZFPUTC fputc
#   define    QES_ZFGETS fgets
#   define    QES_ZFGETC fgetc
#   define    QES_ZFUNGETC fungetc
#   define    QES_ZERR ferror
#   define    QES_ZEOF feof
#   define    QES_ZBUFFER fbuffer
#   define    QES_ZSEEK fseek
#   define    QES_ZTELL ftell
#   define    QES_ZREWIND frewind
#endif

typedef enum __rwmode {
    RW_UNKNOWN,
    RW_READ,
    RW_WRITE,
    RW_READWRITE,
} rwmode_t;

/*  INLINE FUNCTIONS */

/*
 * Memory allocation/deallocation
 */
static inline void *
qes_calloc_ (size_t n, size_t size, qes_errhandler_func onerr, const char *file,
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
#define qes_calloc(n, sz) \
    qes_calloc_(n, sz, QES_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define qes_calloc_errnil(n, sz) \
    qes_calloc_(n, sz, errnil, __FILE__, __LINE__)
#define qes_calloc_errprint(n, sz) \
    qes_calloc_(n, sz, errprint, __FILE__, __LINE__)
#define qes_calloc_errprintexit(n, sz) \
    qes_calloc_(n, sz, errprintexit, __FILE__, __LINE__)

static inline void *
qes_malloc_ (size_t size, qes_errhandler_func onerr, const char *file, int line)
{
    void *ret = malloc(size);
    if (ret == NULL) {
        (*onerr)("malloc returned NULL -- Out of memory", file, line);
        return NULL;
    } else {
        return ret;
    }
}
#define qes_malloc(sz) \
    qes_malloc_(sz, QES_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define qes_malloc_errnil(sz) \
    qes_malloc_(sz, errnil, __FILE__, __LINE__)
#define qes_malloc_errprint(sz) \
    qes_malloc_(sz, errprint, __FILE__, __LINE__)
#define qes_malloc_errprintexit(sz) \
    qes_malloc_(sz, errprintexit, __FILE__, __LINE__)

static inline void *
qes_realloc_ (void *data, size_t size, qes_errhandler_func onerr, const char *file,
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

#define qes_realloc(ptr, sz) \
    qes_realloc_(ptr, sz, QES_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define qes_realloc_errnil(ptr, sz) \
    qes_realloc_(ptr, sz, errnil, __FILE__, __LINE__)
#define qes_realloc_errprint(ptr, sz) \
    qes_realloc_(ptr, sz, errprint, __FILE__, __LINE__)
#define qes_realloc_errprintexit(ptr, sz) \
    qes_realloc_(ptr, sz, errprintexit, __FILE__, __LINE__)
#define qes_free(data)               \
    STMT_BEGIN                      \
    if (data != NULL) {             \
        free(data);                 \
        data = NULL;                \
    }                               \
    STMT_END

#endif /* QES_UTIL_H */
