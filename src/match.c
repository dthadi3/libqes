/*
 * =====================================================================================
 *
 *       Filename:  match.c
 *
 *    Description:  Functions to match sequences
 *
 *        Version:  1.0
 *        Created:  10/11/13 19:28:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include "match.h"
#include "kmlib.h"

/*
    Function matchn:

    Gives the number of mismatches between two sequences between two sequences
*/

size_t
matchnmax  (const char    *haystack,
            const char     *needle,
            size_t          len,
            size_t          max)
{
    /* check seq lengths */
    if (strnlen(haystack, len) < strnlen(needle, len)) {
        fprintf(stderr, "needle longer than haystack at %i in %s\n",
                __LINE__, __func__);
#ifdef  KMLIB_DEBUG
        fprintf(stderr, "\thaystack:\t'%s'\n", haystack);
        fprintf(stderr, "\tneedle:\t'%s'\n", needle);
        fprintf(stderr, "\tlen:\t'%s'\n", len);
        fprintf(stderr, "\tmax:\t'%s'\n", max);
#endif     /* -----  not FDB_DEBUG  ----- */
    }

    /* only check for matches up till needle length */
    if (strnlen(needle, len) < len) {
        len = strnlen(needle, len);
    }

    if (!strncmp(needle, haystack, len)){
        return 0;
    }
    else {
        size_t mismatches = 0;
        for (size_t iii = 0; iii < len; iii++) {
            if (needle[iii] != haystack[iii]) {
                mismatches++;
            }
        }
        return mismatches;
    }
}


/*
    Function findn:

*/

size_t
matchn     (const char     *haystack,
            const char     *needle,
            size_t          len)
{

    /* check seq lengths */
    if (strnlen(haystack, len) < strnlen(needle, len)) {
        ERROR("needle longer than haystack in matchn\n")
    }

    /* only check for matches up till needle length */
    if (strnlen(needle, len) < len) {
        len = strnlen(needle, len);
    }

    if (!strncmp(needle, haystack, len)){
        return 0;
    }
    else {
        size_t mismatches = 0;
        for (size_t iii = 0; iii < len; iii++) {
            if (needle[iii] != haystack[iii]) {
                mismatches++;
            }
        }
        return mismatches;
    }
}
