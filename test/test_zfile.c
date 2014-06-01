/*
 * ============================================================================
 *
 *       Filename:  test_file.c
 *
 *    Description:  Test zfile
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

#include <kmzfile.h>


void
test_zfopen (void *ptr)
{
    zfile_t *file = NULL;
    zfile_t *badfile = NULL;
    char *fname = NULL;

    (void) ptr;
    /* Test file opening for reading */
    fname = find_data_file("loremipsum.txt");
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    tt_ptr_op(file, !=, NULL);
    tt_int_op(file->mode, ==, RW_READ);
    zfclose(file);
    free(fname);
    /* test zipped file opening for reading */
    fname = find_data_file("loremipsum.txt.gz");
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    tt_ptr_op(file, !=, NULL);
    tt_int_op(file->mode, ==, RW_READ);
    zfclose(file);
    free(fname);
    /* read with non-existant file */
    fname = get_writable_file();
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    tt_ptr_op(file, ==, NULL);
    clean_writable_file(fname);
    fname = NULL;
    /* writing with gziped file */
    fname = get_writable_file();
    tt_assert(fname != NULL);
    file = zfopen(fname, "w");
    tt_ptr_op(file, !=, NULL);
    tt_int_op(file->mode, ==, RW_WRITE);
    clean_writable_file(fname);
    fname = NULL;
    /* With non-existant file path */
    badfile = zfopen("non/existant.file", "w");
    tt_ptr_op(badfile, ==, NULL);

end:
    zfclose(file);
    zfclose(badfile);
    if (fname != NULL) free(fname);
}

void
test_zfclose (void *ptr)
{
    zfile_t *file = NULL;
    zfile_t *nullfile = NULL;
    char *fname = NULL;

    (void) ptr;
    /* Open file */
    fname = find_data_file("loremipsum.txt");
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    tt_assert(file);
    zfclose(file);
    tt_ptr_op(file, ==, NULL);
    /* check with null poitner, ensure no problems are caused. */
    zfclose(nullfile);
    tt_ptr_op(nullfile, ==, NULL);
end:
    zfclose(file);
    free(fname);
}


void
test_zfrewind (void *ptr)
{
    zfile_t *file = NULL;
    size_t bufsize = 1<<10;
    char buffer[bufsize];
    ssize_t res = 0;
    char *fname = NULL;

    (void) ptr;
    /* Open file */
    fname = find_data_file("loremipsum.txt");
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    tt_assert(file);
    while (res != EOF) {
        res = zfreadline(file, buffer, bufsize);
    }
    tt_int_op(file->filepos, ==, loremipsum_fsize);
    tt_int_op(KM_ZTELL(file->fp), ==, loremipsum_fsize);
    tt_assert(file->eof);
    tt_assert(file->feof);
    zfrewind(file);
    tt_int_op(file->filepos, ==, 0);
    tt_assert(!file->eof);
    tt_assert(!file->feof);
    tt_int_op(KM_ZTELL(file->fp), ==, 0);
end:
    zfclose(file);
    free(fname);
}

void
test_zfreadline (void *ptr)
{
    zfile_t *file = NULL;
    size_t bufsize = 1<<10;
    char buffer[bufsize];
    ssize_t res_len = 0;
    off_t orig_filepos = 0;
    int iii;
    char *fname = NULL;

    (void) ptr;
    /* Open file */
    fname = find_data_file("loremipsum.txt");
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    /* Check each line is of the right length, that the length is returned,
     * that the string is as expected, and that file->filepos is updated.
     */
    for (iii = 0; iii < n_loremipsum_lines; iii++) {
        orig_filepos = file->filepos;
        res_len = zfreadline(file, buffer, bufsize);
        tt_int_op(res_len, ==, strlen(buffer));
        tt_int_op(res_len, ==, loremipsum_line_lens[iii]);
        tt_str_op(buffer, ==, loremipsum_lines[iii]);
        tt_int_op(file->filepos - orig_filepos, ==, loremipsum_line_lens[iii]);
    }
    /* Check that a file at EOF returns EOF. */
    tt_int_op(file->filepos, ==, loremipsum_fsize);
    tt_int_op(zfreadline(file, buffer, bufsize), ==, EOF);
    tt_assert(file->eof)
    /* Test with bad parameters */
    tt_int_op(zfreadline(NULL, buffer, bufsize), ==, -2);
    tt_int_op(zfreadline(file, NULL, bufsize), ==, -2);
    tt_int_op(zfreadline(file, buffer, 0), ==, -2);

end:
    zfclose(file);
    free(fname);
}

void
test_zfgetuntil (void *ptr)
{
    zfile_t *file = NULL;
    const size_t bufsize = 1<<10;
    char buffer[bufsize];
    ssize_t res_len = 0;
    size_t expt_len = 0;
    off_t orig_filepos = 0;
    off_t our_filepos = 0;
    int iii;
    const size_t n_delims = 5;
    const int delims[] = {' ', ',', '.', '\n', '\n'};
    const char *delim_words[] = {
        "Lorem ",
        "ipsum dolor sit amet,",
        " consectetur adipiscing elit.",
        " Donec ornare tortor et\n",
        "rhoncus iaculis. Sed suscipit, arcu nec elementum vestibulum, tortor tortor\n",
    };
    char *fname = NULL;

    (void) ptr;
    /* Open file */
    fname = find_data_file("loremipsum.txt");
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    /* Check each token is of the right length, that the length is returned,
     * that the string is as expected, and that file->filepos is updated.
     */
    for (iii = 0; iii < n_delims; iii++) {
        orig_filepos = file->filepos;
        res_len = zfgetuntil(file, delims[iii], buffer, bufsize);
        our_filepos += res_len;
        expt_len = strnlen(delim_words[iii], bufsize);
        tt_int_op(res_len, ==, strnlen(buffer, bufsize));
        tt_int_op(res_len, ==, expt_len);
        tt_str_op(buffer, ==, delim_words[iii]);
        tt_int_op(file->filepos - orig_filepos, ==, expt_len);
        tt_int_op(file->filepos, ==, our_filepos);
    }
    /* Check we can give EOF as the char and make it give us the remainder of
       the file */
    orig_filepos = file->filepos;
    res_len = zfgetuntil(file, EOF, buffer, bufsize);
    expt_len = loremipsum_fsize - our_filepos;
    our_filepos += res_len;
    tt_int_op(res_len, ==, strnlen(buffer, bufsize));
    tt_int_op(res_len, ==, expt_len);
    tt_int_op(file->filepos - orig_filepos, ==, expt_len);
    tt_int_op(file->filepos, ==, our_filepos);
    tt_assert(file->eof)
    tt_int_op(file->filepos, ==, loremipsum_fsize);
    /* Check that a file at EOF returns EOF. */
    tt_int_op(file->filepos, ==, loremipsum_fsize);
    tt_int_op(zfgetuntil(file, '\n', buffer, bufsize), ==, EOF);
    tt_assert(file->eof)
    /* Test with bad parameters */
    KM_ZREWIND(file->fp);
    file->eof = 0;
    file->filepos = 0;
    tt_int_op(zfgetuntil(NULL, '\n', buffer, bufsize), ==, -2);
    tt_int_op(zfgetuntil(file, 256, buffer, bufsize), ==, -2);
    tt_int_op(zfgetuntil(file, '\n', NULL, bufsize), ==, -2);
    tt_int_op(zfgetuntil(file, '\n', buffer, 0), ==, -2);
end:
    zfclose(file);
    if (fname != NULL) free(fname);
}

void
test_zfpeek (void *ptr)
{
    int res = 0;
    zfile_t *file = NULL;
    off_t orig_filepos = 0;
    char *fname = NULL;

    (void) ptr;
    /* Open file and save pos */
    fname = find_data_file("loremipsum.txt");
    tt_assert(fname != NULL);
    file = zfopen(fname, "r");
    orig_filepos = file->filepos;
    /* Peek a char */
    res = zfpeek(file);
    /* Check it's the right char */
    tt_int_op(res, ==, loremipsum_lines[0][0]);
    /* And that the filepos hasn't changed */
    tt_int_op(file->filepos , ==, orig_filepos);
    /* And that the same char is returned again */
    res = zfpeek(file);
    tt_int_op(res, ==, loremipsum_lines[0][0]);
    /* And that it returns an error on being given a null pointer  */
    tt_int_op(zfpeek(NULL), ==, -2);
end:
    zfclose(file);
    if (fname != NULL) free(fname);
}

void
test_zfile_guess_mode (void *ptr)
{
    const char *modes[] = {
        "r", "rb", "rb8", "rT", "rbT",
        "w", "wb", "wb8", "wT", "wbT",
        "a", "ab", "ab8", "aT", "abT",
        "+", "+b", "+b8", "+T", "+bT",
    };
    const int mode_results[] = {
        RW_READ, RW_READ, RW_READ, RW_READ, RW_READ,
        RW_WRITE, RW_WRITE, RW_WRITE, RW_WRITE, RW_WRITE,
        RW_WRITE, RW_WRITE, RW_WRITE, RW_WRITE, RW_WRITE,
        RW_UNKNOWN, RW_UNKNOWN, RW_UNKNOWN, RW_UNKNOWN, RW_UNKNOWN,
    };
    const size_t num_modes = 20;
    size_t iii;
    (void) ptr;
    for (iii = 0; iii < num_modes; iii++) {
        tt_int_op(zfile_guess_mode(modes[iii]), ==, mode_results[iii]);
    }
end:
    ;
}

void
test_zfreadline_realloc (void *ptr)
{
    char *buf = NULL;
    char *smallbuf = NULL;
    const size_t smallbuf_len = 4;
    const size_t buf_len = 1<<10; /* 1024b */
    zfile_t *file = NULL;
    ssize_t ret = 0;
    off_t fpos = 0;
    size_t line_num;
    char *nulcp = NULL;
    size_t tmpsz = buf_len;
    char *fname = NULL;

    (void) ptr;
    /* Open file and save pos */
    fname = find_data_file("loremipsum.txt");
    /* This should always work, so long as you run it from the right dir */
    file = zfopen(fname, "r");
    buf = calloc(buf_len, sizeof(*buf));
    smallbuf = calloc(smallbuf_len, sizeof(*smallbuf));
    tt_assert(file && buf && smallbuf);
    /* Check each line is of the right length, that the length is returned,
     * that the string is as expected, and that file->filepos is updated.
     */
    for (line_num = 0; line_num < n_loremipsum_lines; line_num++) {
        ret = zfreadline_realloc(file, &buf, &tmpsz);
        fpos += ret;
        tt_int_op(fpos, ==, file->filepos);
        tt_str_op(buf, ==, loremipsum_lines[line_num]);
        tt_int_op(strlen(buf), ==, loremipsum_line_lens[line_num]);
        tt_int_op(ret, ==, loremipsum_line_lens[line_num]);
        tt_int_op(buf[ret], ==, '\0');
        tt_int_op(tmpsz, ==, buf_len);
        tt_int_op(tmpsz, >=, kmroundupz(loremipsum_line_lens[line_num]));
    }
    tt_int_op(file->filepos, ==, loremipsum_fsize);
    ret = zfreadline_realloc(file, &buf, &tmpsz);
    tt_assert(file->eof);
    tt_int_op(ret, ==, EOF);
    zfclose(file);
    /*
     *                  Test w/ small buffer
     */
    /* Do the same checks, but with a buffer that needs resizing */
    file = zfopen(fname, "r");
    tmpsz = smallbuf_len;
    fpos = 0;
    for (line_num = 0; line_num < n_loremipsum_lines; line_num++) {
        ret = zfreadline_realloc(file, &smallbuf, &tmpsz);
        fpos += ret;
        tt_int_op(fpos, ==, file->filepos);
        tt_str_op(smallbuf, ==, loremipsum_lines[line_num]);
        tt_int_op(strlen(smallbuf), ==, loremipsum_line_lens[line_num]);
        tt_int_op(ret, ==, loremipsum_line_lens[line_num]);
        tt_int_op(tmpsz, !=, smallbuf_len);
        tt_int_op(tmpsz, >=, kmroundupz(loremipsum_line_lens[line_num]));
        tt_int_op(smallbuf[ret], ==, '\0');
    }
    tt_int_op(file->filepos, ==, loremipsum_fsize);
    /* Test with EOF file */
    tmpsz = buf_len;
    ret = zfreadline_realloc(file, &buf, &tmpsz);
    tt_int_op(ret, ==, EOF);
    tt_str_op(buf, ==,  "");
    tt_int_op(strlen(buf), ==, 0);
    tt_int_op(tmpsz, ==, buf_len);
    tt_assert(file->eof)
    zfclose(file);
    /*
     *                     Test bad things
     */
    /* Null buf. Should alloc a buffer and fill it */
    file = zfopen(fname, "r");
    line_num = 0;
    ret = zfreadline_realloc(file, &nulcp, &tmpsz);
    tt_int_op(ret, ==, loremipsum_line_lens[line_num]);
    tt_str_op(nulcp, ==, loremipsum_lines[line_num]);
    tt_int_op(strlen(nulcp), ==, loremipsum_line_lens[line_num]);
    tt_int_op(tmpsz, ==, __INIT_LINE_LEN);
    /* Null file */
    ret = zfreadline_realloc(NULL, &buf, &tmpsz);
    tt_int_op(ret, ==, -2);
    /* This shouldn't change and is set in the prev. test */
    tt_int_op(tmpsz, ==, __INIT_LINE_LEN);
    /* Both buf & file null */
    ret = zfreadline_realloc(NULL, &nulcp, &tmpsz);
    tt_int_op(ret, ==, -2);
    tt_int_op(tmpsz, ==, __INIT_LINE_LEN);
end:
    if (buf != NULL) free(buf);
    if (smallbuf != NULL) free(smallbuf);
    if (nulcp != NULL) free(nulcp);
    if (file != NULL) zfclose(file);
    free(fname);
}

void test_zfile_ok (void *ptr)
{
    zfile_t *file;
    char *writeable = NULL;
    char *readable = NULL;

    (void) ptr;
    readable = find_data_file("loremipsum.txt");
    writeable = get_writable_file();
    /* Should result in an OK file */
    file = zfopen(readable, "r");
    tt_assert(zfile_ok(file));
    tt_assert(zfile_readable(file));
    zfclose(file);
    file = zfopen("nosuchfile", "r");
    tt_assert(!zfile_ok(file));
    tt_assert(!zfile_readable(file));
    zfclose(file);
    file = zfopen(writeable, "w");
    tt_assert(zfile_ok(file));
    tt_assert(!zfile_readable(file));
    zfclose(file);
end:
    if (file != NULL) zfclose(file);
    clean_writable_file(writeable);
    free(readable);

}

struct testcase_t zfile_tests[] = {
    { "zfopen", test_zfopen,},
    { "zfpeek", test_zfpeek,},
    { "zfreadline", test_zfreadline,},
    { "zfreadline_realloc", test_zfreadline_realloc,},
    { "zfile_guess_mode", test_zfile_guess_mode, },
    { "zfclose", test_zfclose,},
    { "zfrewind", test_zfrewind,},
    { "zfgetuntil", test_zfgetuntil,},
    { "zfile_ok", test_zfile_ok,},
    END_OF_TESTCASES
};
