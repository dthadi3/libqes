/*
 * ============================================================================
 *
 *       Filename:  qes_str.c
 *
 *    Description:  String handling functions
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_str.h"


void
qes_str_print (const struct qes_str *str, FILE *stream)
{
    if (qes_str_ok(str)) {
        fprintf(stream, "%s", str->str);
    }
}

void
qes_str_destroy_cp (struct qes_str *str)
{
    if (str != NULL) qes_free(str->str);
}

void
qes_str_destroy (struct qes_str *str)
{
    qes_str_destroy_cp(str);
    qes_free(str);
}

