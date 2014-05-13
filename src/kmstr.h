/*
 * ============================================================================
 *
 *       Filename:  kmstr.h
 *
 *    Description:  String handling functions
 *
 *        Version:  1.0
 *        Created:  22/03/14 13:00:00
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc, clang
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef KMSTR_H
#define KMSTR_H

#include <kmutil.h>
#include <kmzfile.h>

typedef struct _str {
    char *s;
    size_t l;
    size_t m;
} str_t;

extern void print_str (const str_t *str, FILE *stream);
extern void print_strz (const str_t *str, zfile_t *stream);
extern void destroy_str (str_t *str);
extern str_t *str_from_charptr (const char *cp);
extern str_t *create_str (size_t len);
extern int str_ok (const str_t *str);

#endif /* KMSTR_H */
