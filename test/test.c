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
char *text_file;
char *gz_text_file;
char *fasta_file;
char *fastq_file;
char *gzfastq_file;
char *bzfastq_file;
char *writable_file;
const size_t n_loremipsum_lines = 11;
const size_t loremipsum_fsize = 80+76+80+75+80+79+77+75+69+1+20;
const size_t loremipsum_line_lens[] = {
    80, 76, 80, 75, 80, 79, 77, 75, 69, 1, 20
};
const char *loremipsum_lines[] = {
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec ornare tortor et\n",
    "rhoncus iaculis. Sed suscipit, arcu nec elementum vestibulum, tortor tortor\n",
    "dictum dui, eu sodales magna orci eu libero. Cras commodo, ligula tempor auctor\n",
    "vulputate, eros urna gravida eros, eget congue leo quam quis mi. Curabitur\n",
    "luctus augue nibh, eget vehicula augue commodo eget. Donec condimentum molestie\n",
    "adipiscing. In non purus lacus. Nam nec mollis mauris. Donec rhoncus, diam sit\n",
    "amet rhoncus viverra, lectus risus tincidunt ipsum, in dignissim justo purus\n",
    "eget enim. Fusce congue nulla egestas est auctor faucibus. Integer feugiat\n",
    "molestie leo, a interdum neque pretium nec. Etiam sit amet nibh leo.\n",
    "\n",
    "End of lorem ipsum.\n",
};

const char *first_fastq_read[] = {
    "HWI-ST960:105:D10GVACXX:2:1101:1151:2158",
    "1:N:0: bcd:RPI9 seq:CACGATCAGATC",
    "CACGATCAGATCAANGACATTGAATCTATATGT",
    "JJJJJJJIJHIJCC#4ADFFHHHGHJJJJIJJJ",
};

const size_t first_fastq_len = 33;

struct testgroup_t kmlib_tests[] = {
    {"util/", util_tests},
    {"match/", match_tests},
    {"zfile/", zfile_tests},
    {"seqfile/", seqfile_tests},
    END_OF_GROUPS
};

static int
setup_test_env ()
{
    size_t buflen = 1<<12;
    int len = 0;
    int res = 0;
    char buf[buflen];
    char *tmp = NULL;
    if (data_prefix == NULL) {
        tmp = getenv("KMLIB_TEST_DATA_DIR");
        if (tmp == NULL) {
            data_prefix = strdup(".");
        } else {
            data_prefix = strdup(tmp);
        }
    }
    /* find plaintext file */
    len = snprintf(buf, buflen, "%s/data/loremipsum.txt", data_prefix);
    text_file = strdup(buf);
    buf[len] = '\0';
    /* Check we can access the file. If not, we won't be able to access the
       rest, and vice versa, so we don't bother checking below. */
    if (access(text_file, F_OK) != 0) {
        return 0;
    }
    /* find gziped text file */
    len = snprintf(buf, buflen, "%s/data/loremipsum.txt.gz", data_prefix);
    gz_text_file = strdup(buf);
    buf[len] = '\0';
    /* find unzipped FASTQ */
    len = snprintf(buf, buflen,  "%s/data/test.fastq", data_prefix);
    fastq_file = strdup(buf);
    buf[len] = '\0';
    /* find gzipped FASTQ */
    len = snprintf(buf, buflen, "%s/data/test.fastq.gz", data_prefix);
    gzfastq_file = strdup(buf);
    buf[len] = '\0';
    /* find bzipped FASTQ */
    len = snprintf(buf, buflen, "%s/data/test.fastq.bz2", data_prefix);
    bzfastq_file = strdup(buf);
    buf[len] = '\0';
    /* find unzipped FASTA */
    len = snprintf(buf, buflen, "%s/data/test.fasta", data_prefix);
    fasta_file = strdup(buf);
    buf[len] = '\0';
    /* Find and if we need to make the data output dir */
    len = snprintf(buf, buflen, "%s/data/out", data_prefix);
    out_prefix = strdup(buf);
    if (access(out_prefix, W_OK | X_OK) != 0) {
        res = mkdir(out_prefix, S_IRUSR | S_IWUSR | S_IXUSR);
        if (res != 0 || access(out_prefix, W_OK | X_OK) != 0) {
            if (errno) {
                fprintf(stderr, "Error making output dir '%s'\n", out_prefix);
                fprintf(stderr, "%s\n", strerror(errno));
            }
            return 0;
        }
    }
    buf[len] = '\0';
    return 1;
}

static void
destroy_test_env ()
{
    if(data_prefix != NULL) {free(data_prefix);}
    if(out_prefix != NULL) {free(out_prefix);}
    if(text_file != NULL) {free(text_file);}
    if(gz_text_file != NULL) {free(gz_text_file);}
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
    if (argc>1) {
       data_prefix = strdup(argv[1]);
    }

    if (!setup_test_env()) {
        fprintf(stderr, "Could not access data prefix dir '%s' or dir doesn't contain expected files\n", data_prefix);
        fprintf(stderr, "Please set the KMLIB_TEST_DATA_DIR environmental variable appropriately\n");
        free(data_prefix);
        exit(EXIT_FAILURE);
    }
    res = tinytest_main(argc-1, argv+1, kmlib_tests);
    destroy_test_env();
    return res;
}
