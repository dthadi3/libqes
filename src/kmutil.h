/*
 * ============================================================================
 *
 *       Filename:  kmutil.h
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

#ifndef KMUTIL_H
#define KMUTIL_H

/* #####   HEADER FILE INCLUDES   ########################################## */
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <stdint.h>


/*
 * Cross-platform bollocks. Thanks windows.
 */

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#define km_pathsep "\\"
#else
#define km_pathsep "/"
#include <unistd.h>
#endif

/*
 * Misc constants
 */
#define KM_MAX_FN_LEN = 1<<16
#define	__INIT_LINE_LEN 128 /* Starting point for allocing a str_t */
/*
 * Macro helpers from tor
 */

/* Expands to a syntactically valid empty statement.  */
#define STMT_NIL (void)0

/* Expands to a syntactically valid empty statement, explicitly (void)ing its
 * argument. */
#define STMT_VOID(a) while (0) { (void)(a); }

#ifdef __GNUC__
/* STMT_BEGIN and STMT_END are used to wrap blocks inside macros so that
 * the macro can be used as if it were a single C statement. */
#define STMT_BEGIN (void) ({
#define STMT_END })
#elif defined(sun) || defined(__sun__)
#define STMT_BEGIN if (1) {
#define STMT_END } else STMT_NIL
#else
#define STMT_BEGIN do {
#define STMT_END } while (0)
#endif


/*
 * General helper macros
 */
#define km_likely(x)      __builtin_expect(!!(x), 1)
#define km_unlikely(x)    __builtin_expect(!!(x), 0)


/*
 * Error handling constants
 */


/*
 * Error handling functions
 */

/* use the stdlib exit function by default, during testing we can #define this
   to some kind of error handler if we need to. */
#ifndef  KM_EXIT_FN
#define  KM_EXIT_FN exit
#endif


/* By default, we use this error handler. At compile or include time, we can
   chose another more appropriate one if we need to. */
#ifndef  KM_DEFAULT_ERR_FN
#define  KM_DEFAULT_ERR_FN errprintexit
#endif


/* Valid non-function to pass to libkdm functions */

#define ERRFN_ARGS const char *msg,  const char *file, int line, ...
void errnil(ERRFN_ARGS);
void errprint (ERRFN_ARGS);
void errprintexit (ERRFN_ARGS);
typedef void (*errhandler_t) (const char*, const char *, int, ...);
/*
 * Memory allocation/deallocation
 */
extern void *km_calloc_ (size_t n, size_t size, errhandler_t onerr,
        char *file, int line);
#define km_calloc(n, sz) \
    km_calloc_(n, sz, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define km_calloc_errnil(n, sz) \
    km_calloc_(n, sz, errnil, __FILE__, __LINE__)
#define km_calloc_errprint(n, sz) \
    km_calloc_(n, sz, errprint, __FILE__, __LINE__)
#define km_calloc_errprintexit(n, sz) \
    km_calloc_(n, sz, errprintexit, __FILE__, __LINE__)
extern void *km_malloc_ (size_t size, errhandler_t onerr, char *file,
        int line);
#define km_malloc(sz) \
    km_malloc_(sz, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define km_malloc_errnil(sz) \
    km_malloc_(sz, errnil, __FILE__, __LINE__)
#define km_malloc_errprint(sz) \
    km_malloc_(sz, errprint, __FILE__, __LINE__)
#define km_malloc_errprintexit(sz) \
    km_malloc_(sz, errprintexit, __FILE__, __LINE__)
extern void *km_realloc_ (void *data, size_t size, errhandler_t onerr,
        char *file, int line);
#define km_realloc(ptr, sz, fn) km_realloc_(ptr, sz, fn, __FILE__, __LINE__)
#define km_realloc(ptr, sz) \
    km_realloc_(ptr, sz, KM_DEFAULT_ERR_FN, __FILE__, __LINE__)
#define km_realloc_errnil(ptr, sz) \
    km_realloc_(ptr, sz, errnil, __FILE__, __LINE__)
#define km_realloc_errprint(ptr, sz) \
    km_realloc_(ptr, sz, errprint, __FILE__, __LINE__)
#define km_realloc_errprintexit(ptr, sz) \
    km_realloc_(ptr, sz, errprintexit, __FILE__, __LINE__)
#define km_free(data)               \
    STMT_BEGIN                      \
    if (data != NULL) {             \
        free(data);                 \
        data = NULL;                \
    }                               \
    STMT_END

/*
 * Bit fiddling and maths hacks
 */


/* Flogged from http://stackoverflow.com/a/1322548 and
   http://graphics.stanford.edu/~seander/bithacks.html, and kseq.h */
/* Round a 32-bit int up to nearest base-2 number */
#define	kmroundup32(v) (((v) & ((v) - 1)) == 0) ? /* Power of 2 */ \
        (                               \
            (v)|=(v)>>1,                \
            (v)|=(v)>>2,                \
            (v)|=(v)>>4,                \
            (v)|=(v)>>8,                \
            (v)|=(v)>>16,               \
            ++(v)                       \
        ) : (                           \
            --(v),                      \
            (v)|=(v)>>1,                \
            (v)|=(v)>>2,                \
            (v)|=(v)>>4,                \
            (v)|=(v)>>8,                \
            (v)|=(v)>>16,               \
            ++(v)                       \
        )

/* Round a 64-bit int up to nearest base-2 number */
#define	kmroundup64(v) (((v) & ((v) - 1)) == 0) ? /* Power of 2 */ \
        (                               \
            (v)|=(v)>>1,                \
            (v)|=(v)>>2,                \
            (v)|=(v)>>4,                \
            (v)|=(v)>>8,                \
            (v)|=(v)>>16,               \
            (v)|=(v)>>32,               \
            ++(v)                       \
        ) : (                           \
            --(v),                      \
            (v)|=(v)>>1,                \
            (v)|=(v)>>2,                \
            (v)|=(v)>>4,                \
            (v)|=(v)>>8,                \
            (v)|=(v)>>16,               \
            (v)|=(v)>>32,               \
            ++(v)                       \
        )
extern size_t kmroundupz(size_t sz);


#define	KM_ZTYPE_GZIP			/*  we default to zlib.h */

/* Definitions to make changing fp type easy */
#ifdef  KM_ZTYPE_GZIP
#   include <zlib.h>
#   define	KM_ZTYPE gzFile
#   define	KM_ZOPEN gzopen
#   define	KM_ZDOPEN gzdopen
#   define	KM_ZCLOSE gzclose
#   define	KM_ZREAD gzread
#   define	KM_ZWRITE gzwrite
#   define	KM_ZFLUSH gzflush
#   define	KM_ZFPRINTF gzprintf
#   define	KM_ZFPUTS gzputs
#   define	KM_ZFPUTC gzputc
#   define	KM_ZFGETS gzgets
#   define	KM_ZFGETC gzgetc
#   define	KM_ZERR gzerror
#   define	KM_ZEOF gzeof
#   define	KM_ZBUFFER gzbuffer
#   define	KM_ZSEEK gzseek
#   define	KM_ZTELL gztell
#   define	KM_ZREWIND gzrewind
#elif defined(KM_ZTYPE_BZIP)
#   include <bzlib.h>
#   define	KM_ZTYPE BZFILE*
#   define	KM_ZREAD bzread
#   define	KM_ZWRITE bzwrite
#   define	KM_ZERR bzerror
#   define	KM_ZOPEN bzopen
#   define	KM_ZCLOSE bzclose
#elif defined(KM_ZTYPE_NONE)
#   define	KM_ZTYPE FILE*
#   define	KM_ZREAD fread
#   define	KM_ZERR ferror
#   define	KM_ZOPEN fopen
#   define	KM_ZCLOSE fclose
#endif

#endif /* KMUTIL_H */
