/*
 * ============================================================================
 *
 *       Filename:  test_helpers.c
 *
 *    Description:  Tests of test/helpers.c
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"


static void
test_crc32_file(void *data)
{
    char *crc = NULL;
    char *fname = NULL;

    (void) data;
    fname = find_data_file("loremipsum.txt");
    crc = crc32_file(fname);
    tt_str_op(crc, ==, "9f20f7ec");
end:
    free(fname);
    free(crc);
}

static void
test_filecmp(void *data)
{
    char *fname1 = NULL;
    char *fname2 = NULL;

    (void) data;
    fname1 = find_data_file("loremipsum.txt");
    fname2 = find_data_file("loremipsum.txt.gz");
    tt_int_op(filecmp(fname1, fname1), ==, 0);
    tt_int_op(filecmp(fname1, fname2), ==, 1);
    tt_int_op(filecmp(NULL, fname2), ==, -1);
    tt_int_op(filecmp("/does/not/exist/", fname2), ==, -1);

end:
    free(fname1);
    free(fname2);
}

struct testcase_t helper_tests[] = {
    { "crc32_file", test_crc32_file, 0, NULL, NULL},
    { "filecmp", test_filecmp, 0, NULL, NULL},
    END_OF_TESTCASES
};
