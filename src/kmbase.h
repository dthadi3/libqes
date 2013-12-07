/*
 * ===========================================================================
 *
 *       Filename:  kmbase.h
 *
 *    Description:  Base shared functions for all kmlib modules
 *
 *        Version:  1.0
 *        Created:  12/11/13 13:08:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray
 *
 * ===========================================================================
 */

#ifndef KMBASE_H
#define KMBASE_H

/* #####   HEADER FILE INCLUDES   ########################################## */
#include <stdlib.h>
#include <stdio.h>


/* #####   EXPORTED MACROS   ############################################### */
/* constants */

/* Leave this undefined and define with with `gcc -DKMLIB_DEBUG`
#define                     KMLIB_DEBUG
*/
#define                     KMLIB_LINEWIDTH     79


/* helpers */
#define __ERROR(MSG)        fprintf(stderr, MSG);
#define FATAL_ERROR(MSG)    ERROR(MSG) exit(EXIT_FAILURE);

#define KM_ERROR(MSG)       fprintf(stderr, "%s\n\tIn %s at %i in %s\n",      \
                                    MSG, __func__, __LINE__, __FILE__);

#endif /* KMBASE_H */
