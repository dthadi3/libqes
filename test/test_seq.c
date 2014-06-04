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
    /* invalidate comment, should still pass */
    destroy_str_cp(&seq->comment);
    tt_assert(seq_ok_no_comment(seq));
    /* invalidate name, should make seq_ok_no_comment fail */
    destroy_str_cp(&seq->name);
    tt_assert(!seq_ok_no_comment(seq));
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
    /* invalidate qual, should pass */
    destroy_str_cp(&seq->qual);
    tt_assert(seq_ok_no_qual(seq));
    /* invalidate name, should make seq_ok_no_qual fail */
    destroy_str_cp(&seq->name);
    tt_assert(!seq_ok_no_qual(seq));
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
    /* invalidate comment, should still pass */
    destroy_str_cp(&seq->comment);
    tt_assert(seq_ok_no_comment(seq));
    /* invalidate qual, should pass */
    destroy_str_cp(&seq->qual);
    tt_assert(seq_ok_no_comment_or_qual(seq));
    /* invalidate name, should make seq_ok_no_comment_or_qual fail */
    destroy_str_cp(&seq->name);
    tt_assert(!seq_ok_no_comment_or_qual(seq));
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
    seq = NULL;
    destroy_seq(seq);
end:
    destroy_seq(seq);
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
    END_OF_TESTCASES
};
