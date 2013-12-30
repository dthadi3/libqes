/*
 * ===========================================================================
 *
 *       Filename:  kmbase.h
 *
 *    Description:  Base shared functions for all kmlib modules
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *                                [include word penguin in subject]
 *
 * ===========================================================================
 */

#ifndef KMBASE_H
#define KMBASE_H

/* #####   HEADER FILE INCLUDES   ########################################## */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stddef.h>
#include <stdint.h>


/* #####   EXPORTED MACROS   ############################################### */
/* constants */
#define	__FP_TYPE_GZIP			/*  */


/* Definitions to make changing fp type easy */
#ifdef  __FP_TYPE_GZIP
#   include <zlib.h>
#   define	__FP_TYPE gzFile
#   define	__FP_READ gzread
#   define	__FP_ERR gzerror
#   define	__FP_OPEN gzopen
#   define	__FP_CLOSE gzclose
#endif     /* -----  not __KMFILE_TYPE_GZIP  ----- */


/* Leave this undefined and define with with `gcc -DKMLIB_DEBUG`
#define                     KMLIB_DEBUG
*/
#define                     KMLIB_LINEWIDTH     79

/* helpers */
#define __ERROR(MSG)        fprintf(stderr, MSG);
#define FATAL_ERROR(MSG)    __ERROR(MSG) exit(EXIT_FAILURE);

#define KM_ERROR(MSG)       fprintf(stderr, "%s\n\tIn %s at line %i in %s\n", \
                                    MSG, __func__, __LINE__, __FILE__);

#define CHECK_ALLOC(pointer) if(pointer == NULL)                              \
    {fprintf(stderr, "Failed to allocate memory at ln %i\n", __LINE__);}
#define FREE(pointer) if(pointer != NULL) {free(pointer); pointer = NULL;}


#endif /* KMBASE_H */
