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
test_destroy_seq (void *ptr)
{
    seq_t *seq = NULL;
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
    { "create_seq", test_create_seq,},
    { "create_seq_no_qual", test_create_seq_no_qual,},
    { "create_seq_no_qual_or_comment", test_create_seq_no_qual_or_comment,},
    { "destroy_seq", test_destroy_seq,},
    END_OF_TESTCASES
};
