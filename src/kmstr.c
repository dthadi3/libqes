/*
 * ============================================================================
 *
 *       Filename:  kmstr.c
 *
 *    Description:  String handling functions
 *
 *        Version:  1.0
 *        Created:  22/03/14 13:02:11
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "kmstr.h"

inline int
str_ok (const str_t *str)
{
    return (str != NULL && str->s != NULL && str->m > 0 && str->l > 0);
}

inline void
print_str (const str_t *str, FILE *stream)
{
    if (str_ok(str)) {
        fprintf(stream, "%s", str->s);
    }
}

inline void
print_strz (const str_t *str, zfile_t *stream)
{
    KM_ZWRITE(stream->fp, str->s, str->l);
}

inline void
destroy_str (str_t *str)
{
    km_free(str->s);
    km_free(str);
}

inline str_t *
str_from_charptr (const char *cp)
{
    size_t len = strlen(cp);
    str_t *str = create_str(len + 1); /* Enough space for cp + '\0' */
    memcpy(str->s, cp, len);
    str->l = len;
    return str;
}

inline str_t *
create_str (size_t len)
{
    str_t *str = km_calloc(1, sizeof(*str), errprintexit);
    str->l = 0;
    str->s = km_calloc(__INIT_LINE_LEN, sizeof(*str->s), errprintexit);
    str->m = __INIT_LINE_LEN;
    return str;
}
