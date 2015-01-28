/*
 * ============================================================================
 *
 *       Filename:  qes_config.h.in
 *
 *    Description:  Define various things from CMake.
 *
 *        Created:  15/08/14 12:09:59
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef QES_CONFIG_H
#define QES_CONFIG_H

#define LIBQES_VERSION "0.1.12";
#define GETLINE_FOUND
#define STRNDUP_FOUND
#define GETPAGESIZE_FOUND
#undef GZBUFFER_FOUND
#define OPENMP_FOUND
#define MALLOC_H_FOUND

/* Definitions to make changing fp type easy */
#if 0
#   include <zlib.h>
#   define QES_ZTYPE gzFile
#   define QES_ZOPEN gzopen
#   define QES_ZDOPEN gzdopen
#   define QES_ZCLOSE gzclose
#   define QES_ZREAD gzread
#   define QES_ZWRITE gzwrite
#   define QES_ZFLUSH gzflush
#   define QES_ZFPRINTF gzprintf
#   define QES_ZFPUTS gzputs
#   define QES_ZFPUTC gzputc
#   define QES_ZFGETS gzgets
#   define QES_ZFGETC gzgetc
#   define QES_ZFUNGETC gzungetc
#   define QES_ZERR gzerror
#   define QES_ZEOF gzeof
#ifdef GZBUFFER_FOUND
#   define QES_ZBUFFER gzbuffer
#endif
#   define QES_ZSEEK gzseek
#   define QES_ZTELL gztell
#   define QES_ZREWIND gzrewind
#else
#   define QES_ZTYPE FILE*
#   define QES_ZOPEN fopen
#   define QES_ZCLOSE fclose
#   define QES_ZDOPEN fdopen
#   define QES_ZCLOSE fclose
#   define QES_ZREAD(fp, str, l) fread(str, 1, l, fp)
#   define QES_ZWRITE(fp, str, l) fwrite(str, 1, l, fp)
#   define QES_ZFLUSH fflush
#   define QES_ZFPRINTF fprintf
#   define QES_ZFPUTS(fp, s) fputs(s, fp)
#   define QES_ZFPUTC(fp, s) fputc(s, fp)
#   define QES_ZFGETS(fp, s) fgets(s, fp)
#   define QES_ZFGETC fgetc
#   define QES_ZFUNGETC fungetc
#   define QES_ZERR ferror
#   define QES_ZEOF feof
#   define QES_ZBUFFER fbuffer
#   define QES_ZSEEK fseek
#   define QES_ZTELL ftell
#   define QES_ZREWIND frewind
#endif

#endif /* QES_CONFIG_H */
