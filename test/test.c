/*
 * ============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Tests for kmlib
 *
 *        Version:  1.0
 *        Created:  11/08/13 21:34:38
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"


char *data_prefix;
char *out_prefix;
char *fasta_file;
char *fastq_file;
char *gzfastq_file;
char *bzfastq_file;

struct testgroup_t kmlib_tests[] = {
    {"util/", util_tests},
    {"match/", match_tests},
    END_OF_GROUPS
};

void
__setup_test_env ()
{
    char buf[1<<12];
    char *tmp = NULL;
    tmp = getenv("KMLIB_TEST_DATA_DIR");
    if (tmp == NULL) {
        data_prefix = strdup(".");
    } else {
        data_prefix = strdup(tmp);
    }

    sprintf(buf, "%s/data/test.fastq", data_prefix);
    fastq_file = strdup(buf);
    bzero(buf, 1<<12);

    sprintf(buf, "%s/data/test.fastq.gz", data_prefix);
    gzfastq_file = strdup(buf);
    bzero(buf, 1<<12);

    sprintf(buf, "%s/data/test.fastq.bz2", data_prefix);
    bzfastq_file = strdup(buf);
    bzero(buf, 1<<12);

    sprintf(buf, "%s/data/test.fasta", data_prefix);
    fasta_file = strdup(buf);
    bzero(buf, 1<<12);

    sprintf(buf, "%s/data/out", data_prefix);
    out_prefix = strdup(buf);
    bzero(buf, 1<<12);
}

void
__destroy_test_env ()
{
    if(data_prefix != NULL) {free(data_prefix);}
    if(out_prefix != NULL) {free(out_prefix);}
    if(fasta_file != NULL) {free(fasta_file);}
    if(fastq_file != NULL) {free(fastq_file);}
    if(gzfastq_file != NULL) {free(gzfastq_file);}
    if(bzfastq_file != NULL) {free(bzfastq_file);}
}

/*
 * ===  FUNCTION  =============================================================
 *         Name:  main
 *  Description:  Run all tests
 * ============================================================================
 */
int
main (int argc, const char *argv[])
{
    int res;
    __setup_test_env ();
    res = tinytest_main(argc, argv, kmlib_tests);
    __destroy_test_env ();
    return res;
}
