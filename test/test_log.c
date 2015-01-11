/*
 * ============================================================================
 *
 *       Filename:  test_log.c
 *    Description:  Test logging
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "tests.h"
#include <qes_util.h>
#include <qes_log.h>


static void
test_qes_logger(void *ptr)
{
    int res = 0;
    struct qes_logger *logger = NULL;

    (void) ptr;
    /* Test that when created, the logger has logical initial values */
    logger = qes_logger_create();
    tt_ptr_op(logger, !=, NULL);
    tt_ptr_op(logger->destinations, ==, NULL);
    tt_ptr_op(logger->name, ==, NULL);
    tt_int_op(logger->n_destinations, ==, 0);
    tt_int_op(logger->level, ==, QES_LOG_DEBUG);
    tt_int_op(logger->lock, ==, 0);

    /* Add a destination, check it is there */
    res = qes_logger_add_destination(logger, stderr, QES_LOG_ERROR);
    tt_int_op(res, ==, 0);
    tt_ptr_op(logger->destinations, !=, NULL);
    tt_ptr_op(logger->destinations[0].stream, ==, stderr);
    tt_int_op(logger->destinations[0].level, ==, QES_LOG_ERROR);
    tt_int_op(logger->n_destinations, ==, 1);
    qes_logger_destroy(logger);
    tt_ptr_op(logger, ==, NULL);
end:
    if (logger != NULL) free(logger);
}

struct testcase_t qes_log_tests[] = {
    { "qes_logger", test_qes_logger, 0, NULL, NULL},
    END_OF_TESTCASES
};
