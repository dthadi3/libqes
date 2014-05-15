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

typedef struct _str {
    char *s;
    size_t l;
    size_t m;
} str_t;


/*===  FUNCTION  ============================================================*
Name:           str_ok
Parameters:     const str_t *str: `str_t` ref to check.
Description:    Checks if a string is OK to use, i.e. checks that `str->s` is
                a valid `char` array.
Returns:        An `int` that evaluates to true or false.
 *===========================================================================*/
extern int str_ok (const str_t *str);


/*===  FUNCTION  ============================================================*
Name:           create_str
Parameters:     size_t len: Initial capacity of `str_t`.
Description:    Create a `str_t` on the heap with room for `len` characters.
Returns:        `str_t *` Pointer to a memory address.
 *===========================================================================*/
extern str_t *create_str (size_t len);

/*===  FUNCTION  ============================================================*
Name:           init_str
Parameters:     str_t *str: String to initialise (normally on the stack).
                size_t len: Initial capacity of `str_t`.
Description:    Initialise a `str_t` (typically on the stack) with room for
                `len` characters.
Returns:        void
 *===========================================================================*/
extern void init_str (str_t *str, size_t len);


extern int str_fill_charptr_len (str_t *str, const char *cp,
        const size_t len);
extern int str_fill_charptr (str_t *str, const char *cp);

/*===  FUNCTION  ============================================================*
Name:           str_nullify
Paramters:      str_t *str: `str_t` to nullify.
Description:    Invalidates a `str_t` without freeing the `char []`.
Returns:        int: True or False, indicating success or failure.
 *===========================================================================*/
extern int str_nullify(str_t *str);

extern void print_str (const str_t *str, FILE *stream);

/*===  FUNCTION  ============================================================*
Name:           destroy_str
Paramters:      str_t *: `str_t` to destroy.
Description:    Frees `str->s` and the str_t struct itself.
Returns:        void
 *===========================================================================*/
extern void destroy_str (str_t *str);

/*===  FUNCTION  ============================================================*
Name:           destroy_str_cp
Paramters:      str_t *: String to destrop
Description:    Frees `str->s` without freeing the str_t struct itself. For use
                on `str_t`s allocated on the stack.
Returns:        void
 *===========================================================================*/
extern void destroy_str_cp (str_t *str);


#endif /* KMSTR_H */
