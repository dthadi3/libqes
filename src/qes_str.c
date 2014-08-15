/*
 * ============================================================================
 *
 *       Filename:  qes_str.c
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

#include "qes_str.h"

inline void
print_str (const struct qes_str *str, FILE *stream)
{
    if (qes_likely(qes_str_ok(str))) {
        fprintf(stream, "%s", str->s);
    }
}

inline void
destroy_str_cp (struct qes_str *str)
{
    if (qes_unlikely(str != NULL)) qes_free(str->s);
}

inline void
destroy_str (struct qes_str *str)
{
    destroy_str_cp(str);
    qes_free(str);
}

inline int
qes_str_nullify (struct qes_str *str)
{
    if (qes_unlikely(str == NULL)) return 0;
    str->s[0] = '\0';
    str->l = 0;
    return 1;
}

