/*
 * ============================================================================
 *
 *       Filename:  logdemo.c
 *    Description:  Demontrate libqes logging
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include <qes_log.h>

int
main (int argc, char *argv[])
{
    struct qes_logger *logger = qes_logger_create();

    (void) argc;
    (void) argv;

    qes_logger_init(logger, "Test Logger", QES_LOG_DEBUG);
    qes_logger_add_destination_formatted(logger, stdout, QES_LOG_DEBUG,
                                         &qes_log_formatter_pretty);

    qes_log_message_debug(logger, "Debug message, nice and quiet\n");
    qes_log_message_info(logger, "Informative message, clearer\n");
    qes_log_message_warning(logger, "Warning message, pay attention!\n");
    qes_log_message_error(logger, "Error message, something's gone wrong\n");
    qes_log_message_fatal(logger, "Fatal message, I'm leaving now\n");

    qes_logger_destroy(logger);
    return EXIT_SUCCESS;
}
