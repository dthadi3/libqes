/*
 * ============================================================================
 *
 *       Filename:  test_sequtil.c
 *
 *    Description:  Tests for the sequtil module
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"
#include <qes_sequtil.h>


static void
test_qes_sequtil_translate_codon (void *ptr)
{
    size_t iii;
    size_t jjj;
    char *cdn = NULL;
    char aa = 0;

    (void) ptr;
    for (iii = 0; iii < n_codons; iii++) {
        aa = qes_sequtil_translate_codon(codon_list[iii]);
        tt_assert_op_type(aa, ==, aa_list[iii], char, "%c");
    }
    tt_int_op(qes_sequtil_translate_codon("XACACA"), ==, -1);
    tt_int_op(qes_sequtil_translate_codon("A"), ==, -1);
    tt_int_op(qes_sequtil_translate_codon(NULL), ==, -1);
    /* Try with mutations */
    for (iii = 0; iii < n_codons; iii++) {
        for (jjj = 0; jjj < 3; jjj++) {
            cdn = strdup(codon_list[iii]);
            cdn[jjj] = 'N';
            aa = qes_sequtil_translate_codon(cdn);
            tt_assert_op_type(aa, ==, 'X', char, "%c");
            free(cdn);
            cdn = NULL;
        }
    }
end:
    if (cdn != NULL) free(cdn);
}

struct testcase_t qes_sequtil_tests[] = {
    { "qes_sequtil_translate_codon", test_qes_sequtil_translate_codon, 0, NULL, NULL},
    END_OF_TESTCASES
};
