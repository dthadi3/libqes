/*
 * ============================================================================
 *
 *       Filename:  test_file.c
 *
 *    Description:  Test kmfile
 *
 *        Version:  1.0
 *        Created:  22/03/14 12:16:56
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#include "tests.h"

void
test_create_kmfile (void *ptr)
{
    kmfile *file = NULL;
    kmfile *fafile = NULL;
    kmfile *zfile = NULL;
    kmfile *badfile = NULL;
    const char *fname = NULL;
    (void) ptr;
    /* Test file creation */
    fname = find_data_file("test.fastq");
    file = create_kmfile(fname);
    tt_ptr_op(file, !=, NULL);
    tt_int_op(file->mode, ==, '@');
    free(fname);
    /* with FASTA file */
    fname = find_data_file("test.fasta");
    fafile = create_kmfile(fname);
    tt_ptr_op(fafile, !=, NULL);
    tt_int_op(fafile->mode, ==, '@');
    free(fname);
    /* with gzipped FASTA file */
    fname = find_data_file("test.fastq.gz");
    zfile = create_kmfile(fname);
    tt_ptr_op(zfile, !=, NULL);
    tt_int_op(zfile->mode, ==, '@');
    /* With bad files */
    badfile = create_kmfile("non/existant.file");
    tt_ptr_op(badfile, ==, NULL);
end:
    free(fname);
    destroy_kmfile(fafile);
    destroy_kmfile(zfile);
    destroy_kmfile(badfile);
}

void
test_kmfile ()
{
    int bytes_read = 0;
    kmfile *file = NULL;
    kmfile *fafile = NULL;
    kmfile *zfile = NULL;
    kmfile *badfile = NULL;
    char *buffer = NULL;
    char *buf_before = NULL;
    char *buf_after = NULL;

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
}

void
test_km_readline_realloc (void *ptr)
{
    char *lorem_fn = NULL;
    char *buf = NULL;
    char *smallbuf = NULL;
    const size_t smallbuf_len = 1<<4;
    FILE *fp = NULL;
    ssize_t ret = 0;
    size_t line_num;
    char *nulcp = NULL;
    FILE *nulfp = NULL;
    char *tmpcp = NULL;
    size_t tmpsz = 0;
    size_t our_bufsize = bufsize;
    size_t our_smallbuf_len = smallbuf_len;
    km_test_err = 0;
    /* This should always work, so long as you run it from the right dir */
    lorem_fn = get_test_filename("loremipsum.txt");
    if (lorem_fn == NULL)
        tt_abort_msg("Broken test - get_test_filename failed\n");
    if ((fp = fopen(lorem_fn, "r")) == NULL) {
        fprintf(stderr, "Could not open test file '%s' -- %s\n",
                lorem_fn, strerror(errno));
        tt_skip();
    } else {
        buf = calloc(our_bufsize, sizeof(*buf));
        smallbuf = calloc(our_smallbuf_len, sizeof(*smallbuf));
    }
    for (line_num = 0; line_num < n_loremipsum_lines; line_num++) {
        ret = km_readline_realloc(&buf, fp, &our_bufsize, &test_err_handler);
        tt_int_op(km_test_err, ==, 0);
        tt_int_op(strncmp(buf, loremipsum_lines[line_num], our_bufsize), ==, 0);
        tt_int_op(strlen(buf), ==, loremipsum_line_lens[line_num]);
        tt_int_op(ret, ==, loremipsum_line_lens[line_num]);
        tt_int_op(our_bufsize, ==, bufsize);
        km_test_err = 0;
    }
    ret = km_readline_realloc(&buf, fp, &our_bufsize, &test_err_handler);
    tt_int_op(km_test_err, ==, 0);
    /* check it leaves  \0 in buf */
    tt_int_op(strncmp(buf, "", our_bufsize), ==, 0);
    tt_int_op(strlen(buf), ==, 0);
    tt_int_op(ret, ==, EOF);
    tt_int_op(our_bufsize, ==, bufsize);
    km_test_err = 0;
    /* Naughty tests that try and make it fail */
    rewind(fp);
    /* Null buf */
    ret = km_readline_realloc(&nulcp, fp, &our_bufsize, &test_err_handler);
    tt_int_op(km_test_err, ==, 3);
    tt_int_op(ret, ==, -2);
    tt_int_op(our_bufsize, ==, bufsize);
    km_test_err = 0;
    /* Null fp */
    ret = km_readline_realloc(&buf, nulfp, &our_bufsize, &test_err_handler);
    tt_int_op(km_test_err, ==, 3);
    tt_int_op(ret, ==, -2);
    tt_int_op(our_bufsize, ==, bufsize);
    km_test_err = 0;
    /* Both buf & fp null */
    ret = km_readline_realloc(&nulcp, nulfp, &our_bufsize, &test_err_handler);
    tt_int_op(km_test_err, ==, 3);
    tt_int_op(ret, ==, -2);
    tt_int_op(our_bufsize, ==, bufsize);
    km_test_err = 0;
    /* Test that should require it to resize the buffer */
    rewind(fp);
    ret = km_readline_realloc(&smallbuf, fp, &our_smallbuf_len,
            &test_err_handler);
    tt_int_op(km_test_err, ==, 0);
    tt_int_op(ret, ==, loremipsum_line_lens[0]);
    tt_int_op(strlen(smallbuf), ==, loremipsum_line_lens[0]);
    tmpsz = loremipsum_line_lens[0];
    tt_int_op(our_smallbuf_len, ==, kmroundupz(tmpsz));
end:
    if (lorem_fn != NULL) free(lorem_fn);
    if (buf != NULL) free(buf);
    if (smallbuf != NULL) free(smallbuf);
    if (fp != NULL) fclose(fp);
}
