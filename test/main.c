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
 *                                [include word penguin in subject]
 *
 * ============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include <kmlib.h>
#include <kmtest.h>

TEST_INIT()

static char *data_prefix;
static char *out_prefix;
static char *fasta_file;
static char *fastq_file;
static char *gzfastq_file;
static char *bzfastq_file;

static void
test_kmfile                     ()
{ /* {{{ */
    int bytes_read = 0;
    kmfile *file = NULL;
    kmfile *fafile = NULL;
    kmfile *zfile = NULL;
    kmfile *badfile = NULL;
    char *buffer = NULL;
    char *buf_before = NULL;
    char *buf_after = NULL;
    printf("\nTesting kmfile.c functions\n");

    /* Test file creation */
    file = create_kmfile(fastq_file);
    if (file == NULL) { exit(EXIT_FAILURE);}
    TEST_PTR_NONNULL(file, "create_kmfile fastq");
    TEST(file->mode, '@', "kmfile_mode fastq", "%c");

    fafile = create_kmfile(fasta_file);
    if (fafile == NULL) { exit(EXIT_FAILURE);}
    TEST_PTR_NONNULL(fafile, "create_kmfile fasta");
    TEST(fafile->mode, '>', "kmfile_mode fasta", "%c");
    destroy_kmfile(fafile);

    zfile = create_kmfile(fasta_file);
    if (zfile == NULL) { exit(EXIT_FAILURE);}
    TEST_PTR_NONNULL(zfile, "create_kmfile zipped fastq");
    TEST(zfile->mode, '>', "kmfile_mode zipped fastq", "%c");
    destroy_kmfile(zfile);

    badfile = create_kmfile("non/existant.file");
    TEST_PTR(badfile, NULL, "create_kmfile non-existing file");
    destroy_kmfile(badfile);

    /* Test hint_line_length_kmfile */
    TEST_SIZET(hint_line_length_kmfile(file), 76, "hint_line_length_kmfile");

    /* Test peek_ahead_kmfile */
    TEST(peek_ahead_kmfile(file), '@', "peek_ahead_kmfile", "%c");
    /* Test readline_kmfile */
    buffer = calloc(1<<10, 1);
    buf_before = file->bufferiter;
    bytes_read = readline_kmfile(file, &buffer, 1<<10);
    buf_after = file->bufferiter;
    TEST_STR(buffer, "@HWI-ST960:105:D10GVACXX:2:1101:1151:2158"
            " 1:N:0: bcd:RPI9 seq:CACGATCAGATC\n",
            "readline_kmfile string value");
    TEST_INT(bytes_read, 75, "readline_kmfile bytes read");
    TEST_PTR(buf_before + 75, buf_after, "readline_kmfile buffer addresses");
    bytes_read = readline_kmfile(file, &buffer, 1<<10);
    TEST_STR(buffer, "CACGATCAGATCAANGACATTGAATCTATATGT\n",
            "readline_kmfile string value second line");
    FREE(buffer);
    buffer = NULL;

    /* Test __readline_kmfile_keep */
    bytes_read = 0;
    buffer = calloc(1<<10, 1);
    buf_before = file->bufferiter;
    bytes_read = __readline_kmfile_keep(file, &buffer, 1<<10);
    buf_after = file->bufferiter;
    TEST_STR(buffer, "+\n",
            "__readline_kmfile_keep string value");
    TEST_INT(bytes_read, 2, "__readline_kmfile_keep bytes read");
    TEST_PTR(buf_before, buf_after, "__readline_kmfile_keep buffer addresses");
    FREE(buffer);
    buffer = NULL;

    /* Clean up */
    destroy_kmfile(file);
    file = NULL;
    printf("\nDone with kmfile.c functions\n\n");
} /* }}} */


static void
test_kmmatch                    ()
{ /* {{{ */
    printf("\nTesting kmmatch.c functions\n");

    TEST_SIZET(hamming("ACTTG", "ACTGG"), 1, "hamming");
    TEST_SIZET(hamming("ACTTG", "ACTGGA"), SIZE_MAX,
            "hamming seqlen not equal");

    TEST_SIZET(hamming_max("ACTTG", "ACTGG", 1), 1,
            "hamming_max with max > hamming");
    TEST_SIZET(hamming_max("ACTTG", "ACTGG", 0), 0,
            "hamming_max with max < hamming");
    TEST_SIZET(hamming_max("ACTTG", "ACTGGA", 1), SIZE_MAX,
            "hamming_max seqlen not equal");

    printf("\nDone with kmmatch.c functions\n\n");
} /* }}} */


static void
test_kmseq                      ()
{
    kmseq *seq = NULL;
    kmseq *badseq = NULL;
    kmseq *read_seq = NULL;
    kmfile *file = NULL;
    kmfile *fafile = NULL;
    file = create_kmfile(fastq_file);

    printf("\nTesting kmseq.c functions\n");

    /* Test seq creation */
    seq = create_kmseq();
    TEST_PTR_NONNULL(seq, "create_kmseq");
    TEST_PTR(seq->name, NULL, "create_kmseq name");
    TEST_PTR(seq->seq, NULL, "create_kmseq seq");
#ifdef  KMSEQ_QUAL
    TEST_PTR(seq->seq, NULL, "create_kmseq qual");
#endif

    /* Test filling */
#ifdef  KMSEQ_QUAL
    TEST_INT(fill_kmseq(seq, "test1", "AGCCT", "CCFFC"), 0, "fill_kmseq");
#else
    TEST_INT(fill_kmseq(seq, "test1", "AGCCT"), 0, "fill_kmseq");
#endif
    TEST_STR(seq->name, "test1", "fill_kmseq name");
    TEST_STR(seq->seq, "AGCCT", "fill_kmseq seq");
#ifdef  KMSEQ_QUAL
    TEST_STR(seq->qual, "CCFFC", "fill_kmseq qual");
#endif

    badseq = create_kmseq();
    TEST_INT(fill_kmseq(badseq, "test1", "A", "CC"), 1,
            "fill_kmseq bad lengths");

    /* Test reading file */
    read_seq = read_seq_file(file);
    TEST_PTR_NONNULL(read_seq, "read_seq_file");
    TEST_STR(read_seq->name, "HWI-ST960:105:D10GVACXX:2:1101:1151:2158 1:N:0:"
            " bcd:RPI9 seq:CACGATCAGATC", "read_seq_file name");
    TEST_STR(read_seq->seq, "CACGATCAGATCAANGACATTGAATCTATATGT",
            "read_seq_file seq");
#ifdef  KMSEQ_QUAL
    TEST_STR(read_seq->qual, "JJJJJJJIJHIJCC#4ADFFHHHGHJJJJIJJJ",
            "read_seq_file qual");
#endif
    destroy_kmseq(read_seq);


    /* test reading fasta file */
    fafile = create_kmfile(fasta_file);
    read_seq = read_seq_file(fafile);
    TEST_PTR_NONNULL(read_seq, "read_seq_file fasta");
    TEST_STR(read_seq->name, "HWI-ST960:105:D10GVACXX:2:1101:1122:2186 1:N:0:"
            " bcd:RPI8 seq:CACACTTGAATC", "read_seq_file name fasta");
    TEST_STR(read_seq->seq, "CACACTTGAATCCAGTTTAAAGTTAACTCATTG",
            "read_seq_file seq fasta");
#ifdef  KMSEQ_QUAL
    /* TEST_STR(read_seq->qual, "", "read_seq_file qual"); */
#endif
    destroy_kmseq(read_seq);


    destroy_kmfile(fafile);



    destroy_kmseq(seq);
    destroy_kmseq(badseq);
    destroy_kmfile(file);
    printf("\nDone with kmseq.c functions\n\n");
}


static void
__setup_test_env                ()
{ /* {{{ */
    char tmp[1<<12];

    sprintf(tmp, "%s/data/test.fastq", data_prefix);
    fastq_file = strdup(tmp);
    bzero(tmp, 1<<12);

    sprintf(tmp, "%s/data/test.fastq.gz", data_prefix);
    gzfastq_file = strdup(tmp);
    bzero(tmp, 1<<12);

    sprintf(tmp, "%s/data/test.fastq.bz2", data_prefix);
    bzfastq_file = strdup(tmp);
    bzero(tmp, 1<<12);

    sprintf(tmp, "%s/data/test.fasta", data_prefix);
    fasta_file = strdup(tmp);
    bzero(tmp, 1<<12);

    sprintf(tmp, "%s/data/out", data_prefix);
    out_prefix = strdup(tmp);
    bzero(tmp, 1<<12);
} /* }}} */

/*
 * ===  FUNCTION  =============================================================
 *         Name:  main
 *  Description:  Run all tests
 * ============================================================================
 */

int
main                            (int argc, char *argv[])
{ /* {{{ */
#ifdef  KMLIB_DEBUG
    TEST_SELF();
#endif     /* -----  not KMLIB_DEBUG  ----- */

    if (argc > 1) {
        data_prefix = strdup(argv[1]);
    } else {
        data_prefix = strdup(".");
    }
    __setup_test_env();

    test_kmfile();
    test_kmmatch();
    test_kmseq();

    TEST_EXIT();
    if (data_prefix != NULL) {FREE(data_prefix);}
    if (out_prefix != NULL) {FREE(out_prefix);}
    if (fastq_file != NULL) {FREE(fastq_file);}
    if (gzfastq_file != NULL) {FREE(gzfastq_file);}
    if (bzfastq_file != NULL) {FREE(bzfastq_file);}
    if (fasta_file != NULL) {FREE(fasta_file);}
    return EXIT_SUCCESS;
} /* }}} */
