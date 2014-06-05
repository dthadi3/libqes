/*
 * ============================================================================
 *
 *       Filename:  test_seq.c
 *
 *    Description:  Test kmseq.c
 *
 *        Version:  1.0
 *        Created:  04/05/14 15:58:53
 *       Revision:  none
 *        License:  GPLv3+
 *       Compiler:  gcc
 *
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */
#include "tests.h"

#include <kmseq.h>

void
test_create_seq (void *ptr)
{
    seq_t *seq = NULL;
    (void) ptr;
    tt_ptr_op(seq, ==, NULL);
    seq = create_seq();
    tt_ptr_op(seq, !=, NULL);
    tt_ptr_op(seq->name.s, !=, NULL);
    tt_int_op(seq->name.m, >, 0);
    tt_int_op(seq->name.l, ==, 0);
    tt_ptr_op(seq->comment.s, !=, NULL);
    tt_int_op(seq->comment.m, >, 0);
    tt_int_op(seq->comment.l, ==, 0);
    tt_ptr_op(seq->seq.s, !=, NULL);
    tt_int_op(seq->seq.m, >, 0);
    tt_int_op(seq->seq.l, ==, 0);
    tt_ptr_op(seq->qual.s, !=, NULL);
    tt_int_op(seq->qual.m, >, 0);
    tt_int_op(seq->qual.l, ==, 0);
    tt_assert(str_ok(&seq->name));
    tt_assert(str_ok(&seq->comment));
    tt_assert(str_ok(&seq->seq));
    tt_assert(str_ok(&seq->qual));
end:
    destroy_seq(seq);
}

void
test_create_seq_no_qual (void *ptr)
{
    seq_t *seq = NULL;
    (void) ptr;
    tt_ptr_op(seq, ==, NULL);
    seq = create_seq_no_qual();
    tt_ptr_op(seq, !=, NULL);
    tt_ptr_op(seq->name.s, !=, NULL);
    tt_int_op(seq->name.m, >, 0);
    tt_int_op(seq->name.l, ==, 0);
    tt_ptr_op(seq->comment.s, !=, NULL);
    tt_int_op(seq->comment.m, >, 0);
    tt_int_op(seq->comment.l, ==, 0);
    tt_ptr_op(seq->seq.s, !=, NULL);
    tt_int_op(seq->seq.m, >, 0);
    tt_int_op(seq->seq.l, ==, 0);
    tt_ptr_op(seq->qual.s, ==, NULL);
    tt_int_op(seq->qual.m, ==, 0);
    tt_int_op(seq->qual.l, ==, 0);
    tt_assert(str_ok(&seq->name));
    tt_assert(str_ok(&seq->comment));
    tt_assert(str_ok(&seq->seq));
    tt_assert(!str_ok(&seq->qual));
end:
    destroy_seq(seq);
}

void
test_create_seq_no_qual_or_comment (void *ptr)
{
    seq_t *seq = NULL;
    (void) ptr;
    tt_ptr_op(seq, ==, NULL);
    seq = create_seq_no_qual_or_comment();
    tt_ptr_op(seq, !=, NULL);
    tt_ptr_op(seq->name.s, !=, NULL);
    tt_int_op(seq->name.m, >, 0);
    tt_int_op(seq->name.l, ==, 0);
    tt_ptr_op(seq->comment.s, ==, NULL);
    tt_int_op(seq->comment.m, ==, 0);
    tt_int_op(seq->comment.l, ==, 0);
    tt_ptr_op(seq->seq.s, !=, NULL);
    tt_int_op(seq->seq.m, >, 0);
    tt_int_op(seq->seq.l, ==, 0);
    tt_ptr_op(seq->qual.s, ==, NULL);
    tt_int_op(seq->qual.m, ==, 0);
    tt_int_op(seq->qual.l, ==, 0);
    tt_assert(str_ok(&seq->name));
    tt_assert(!str_ok(&seq->comment));
    tt_assert(str_ok(&seq->seq));
    tt_assert(!str_ok(&seq->qual));
end:
    destroy_seq(seq);
}

void
test_seq_ok (void *ptr)
{
    seq_t *seq = NULL;
    (void) ptr;
    /* Test null seq */
    tt_assert(!seq_ok(seq));
    /* Make valid seq */
    seq = create_seq();
    tt_assert(seq_ok(seq));
    /* invalidate name, should fail */
    destroy_str_cp(&seq->name);
    tt_assert(!seq_ok(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok(seq));
    /* invalidate comment, should fail */
    destroy_str_cp(&seq->comment);
    tt_assert(!seq_ok(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok(seq));
    /* invalidate seq, should fail */
    destroy_str_cp(&seq->seq);
    tt_assert(!seq_ok(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok(seq));
    /* invalidate qual, should fail */
    destroy_str_cp(&seq->qual);
    tt_assert(!seq_ok(seq));
    destroy_seq(seq);
    /* Destroy seq, invalidating it */
    destroy_seq(seq);
    tt_assert(!seq_ok(seq));
end:
    destroy_seq(seq);
}

void
test_seq_ok_no_comment (void *ptr)
{
    seq_t *seq = NULL;

    (void) ptr;
    /* Test null seq */
    tt_assert(!seq_ok_no_comment(seq));
    /* Make valid seq */
    seq = create_seq();
    tt_assert(seq_ok_no_comment(seq));
    /* invalidate name, should fail */
    destroy_str_cp(&seq->name);
    tt_assert(!seq_ok_no_comment(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_comment(seq));
    /* invalidate comment, should still pass */
    destroy_str_cp(&seq->comment);
    tt_assert(seq_ok_no_comment(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_comment(seq));
    /* invalidate seq, should fail */
    destroy_str_cp(&seq->seq);
    tt_assert(!seq_ok_no_comment(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_comment(seq));
    /* invalidate qual, should fail */
    destroy_str_cp(&seq->qual);
    tt_assert(!seq_ok_no_comment(seq));
    destroy_seq(seq);
    /* Destroy seq, invalidating it */
    destroy_seq(seq);
    tt_assert(!seq_ok_no_comment(seq));
end:
    destroy_seq(seq);
}

void
test_seq_ok_no_qual (void *ptr)
{
    seq_t *seq = NULL;
    (void) ptr;
    /* Test null seq */
    tt_assert(!seq_ok_no_qual(seq));
    /* Make valid seq */
    seq = create_seq();
    tt_assert(seq_ok_no_qual(seq));
    /* invalidate name, should fail */
    destroy_str_cp(&seq->name);
    tt_assert(!seq_ok_no_qual(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_qual(seq));
    /* invalidate comment, should fail */
    destroy_str_cp(&seq->comment);
    tt_assert(!seq_ok_no_qual(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_qual(seq));
    /* invalidate seq, should fail */
    destroy_str_cp(&seq->seq);
    tt_assert(!seq_ok_no_qual(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_qual(seq));
    /* invalidate qual, should PASS */
    destroy_str_cp(&seq->qual);
    tt_assert(seq_ok_no_qual(seq));
    destroy_seq(seq);
    /* Destroy seq, invalidating it */
    destroy_seq(seq);
    tt_assert(!seq_ok_no_qual(seq));
end:
    destroy_seq(seq);
}

void
test_seq_ok_no_comment_or_qual (void *ptr)
{
    seq_t *seq = NULL;

    (void) ptr;
    /* Test null seq */
    tt_assert(!seq_ok_no_comment_or_qual(seq));
    /* Make valid seq */
    seq = create_seq();
    tt_assert(seq_ok_no_comment_or_qual(seq));
    /* invalidate name, should fail */
    destroy_str_cp(&seq->name);
    tt_assert(!seq_ok_no_comment_or_qual(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_comment_or_qual(seq));
    /* invalidate comment, should still pass */
    destroy_str_cp(&seq->comment);
    tt_assert(seq_ok_no_comment_or_qual(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_comment_or_qual(seq));
    /* invalidate seq, should fail */
    destroy_str_cp(&seq->seq);
    tt_assert(!seq_ok_no_comment_or_qual(seq));
    destroy_seq(seq);
    /* remake */
    seq = create_seq();
    tt_assert(seq_ok_no_comment_or_qual(seq));
    /* invalidate qual, should fail */
    destroy_str_cp(&seq->qual);
    tt_assert(seq_ok_no_comment_or_qual(seq));
    destroy_seq(seq);
    /* Destroy seq, invalidating it */
    destroy_seq(seq);
    tt_assert(!seq_ok_no_comment_or_qual(seq));
end:
    destroy_seq(seq);
}


void
test_destroy_seq (void *ptr)
{
    seq_t *seq = NULL;

    (void) ptr;
    tt_ptr_op(seq, ==, NULL);
    seq = create_seq();
    tt_ptr_op(seq, !=, NULL);
    destroy_seq(seq);
    tt_ptr_op(seq, ==, NULL);
    seq = NULL; /* Best be sure */
    destroy_seq(seq);
end:
    destroy_seq(seq);
}

void
test_seq_fill_funcs(void *ptr)
{
#define CHECK_FILLING(submember, str, len)          \
    seq = create_seq();                             \
    tt_str_op(seq->submember.s, ==, "");            \
    res = seq_fill_ ##submember (seq, str, len);    \
    tt_int_op(res, ==, 1);                          \
    tt_str_op(seq->submember.s, ==, str);           \
    tt_int_op(seq->submember.l, ==, len);           \
    tt_int_op(seq->submember.m, >=, len);           \
    destroy_seq(seq);
#define CHECK_FILLING_FAIL(submember, str, len)     \
    seq = create_seq();                             \
    tt_str_op(seq->submember.s, ==, "");            \
    res = seq_fill_ ##submember (seq, str, len);    \
    tt_int_op(res, ==, 0);                          \
    tt_str_op(seq->submember.s, ==, "");            \
    tt_int_op(seq->submember.l, ==, 0);             \
    destroy_seq(seq);

    seq_t *seq = NULL;
    int res = 0;

    (void) ptr;
    /* These should all work pretty well */
    CHECK_FILLING(name, "HWI_TEST", 8)
    CHECK_FILLING(comment, "abc 123 comment", 15)
    CHECK_FILLING(seq, "ACTG", 4)
    CHECK_FILLING(qual, "IIII", 4)
    /* These should all fail */
    CHECK_FILLING_FAIL(name, NULL, 1)
    CHECK_FILLING_FAIL(name, "BAD", 0)
    CHECK_FILLING_FAIL(comment, NULL, 1)
    CHECK_FILLING_FAIL(comment, "BAD", 0)
    CHECK_FILLING_FAIL(seq, NULL, 1)
    CHECK_FILLING_FAIL(seq, "BAD", 0)
    CHECK_FILLING_FAIL(qual, NULL, 1)
    CHECK_FILLING_FAIL(qual, "BAD", 0)
    tt_int_op(seq_fill_name(NULL, "BAD", 3), ==, 0);
    tt_int_op(seq_fill_comment(NULL, "BAD", 3), ==, 0);
    tt_int_op(seq_fill_seq(NULL, "BAD", 3), ==, 0);
    tt_int_op(seq_fill_qual(NULL, "BAD", 3), ==, 0);

    /* Fill header */
#define CHECK_FILL_HEADER(str, len, nm, nmlen, com, comlen)     \
    seq = create_seq();                                         \
    tt_str_op(seq->name.s, ==, "");                             \
    tt_str_op(seq->comment.s, ==, "");                          \
    res = seq_fill_header(seq, str, len);                       \
    tt_int_op(res, ==, 1);                                      \
    tt_str_op(seq->name.s, ==, nm);                             \
    tt_int_op(seq->name.l, ==, nmlen);                          \
    tt_int_op(seq->name.m, >=, nmlen);                          \
    tt_str_op(seq->comment.s, ==, com);                         \
    tt_int_op(seq->comment.l, ==, comlen);                      \
    tt_int_op(seq->comment.m, >=, comlen);                      \
    destroy_seq(seq);
    CHECK_FILL_HEADER("@HWI_TEST COMM", 14, "HWI_TEST", 8, "COMM", 4)
    CHECK_FILL_HEADER(">HWI_TEST COMM", 14, "HWI_TEST", 8, "COMM", 4)
    CHECK_FILL_HEADER("HWI_TEST COMM", 13, "HWI_TEST", 8, "COMM", 4)
    CHECK_FILL_HEADER("@HWI_TEST", 9, "HWI_TEST", 8, "", 0)
    CHECK_FILL_HEADER(">HWI_TEST", 9, "HWI_TEST", 8, "", 0)
    CHECK_FILL_HEADER("HWI_TEST", 8, "HWI_TEST", 8, "", 0)
    /* Check bad values */
    seq = create_seq();
    tt_int_op(seq_fill_header(NULL, "BAD", 3), ==, 0);
    tt_int_op(seq_fill_header(seq, NULL, 3), ==, 0);
    tt_int_op(seq_fill_header(seq, "BAD", 0), ==, 0);
    destroy_seq(seq);
end:
    destroy_seq(seq);
#undef CHECK_FILLING
#undef CHECK_FILLING_FAIL
#undef CHECK_FILL_HEADER
}


struct testcase_t seq_tests[] = {
    { "create_seq", test_create_seq, 0, NULL, NULL},
    { "create_seq_no_qual", test_create_seq_no_qual, 0, NULL, NULL},
    { "create_seq_no_qual_or_comment", test_create_seq_no_qual_or_comment, 0, NULL, NULL},
    { "seq_ok", test_seq_ok, 0, NULL, NULL},
    { "seq_ok_no_comment", test_seq_ok_no_comment, 0, NULL, NULL},
    { "seq_ok_no_qual", test_seq_ok_no_qual, 0, NULL, NULL},
    { "seq_ok_no_comment_or_qual", test_seq_ok_no_comment_or_qual, 0, NULL, NULL},
    { "destroy_seq", test_destroy_seq, 0, NULL, NULL},
    { "seq_fill", test_seq_fill_funcs, 0, NULL, NULL},
    END_OF_TESTCASES
};
