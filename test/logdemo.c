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
    const char *message = "Test!";
    struct qes_logger *logger = qes_logger_create();

    if (argc > 1) {
        message = argv[1];
    }

    qes_logger_init(logger, "Test Logger", QES_LOG_DEBUG);
    qes_logger_add_destination_formatted(logger, stdout, QES_LOG_DEBUG,
                                         &qes_log_formatter_pretty);

    qes_log_format_debug(logger, "%s\n", message);
    qes_log_format_info(logger, "%s\n", message);
    qes_log_format_warning(logger, "%s\n", message);
    qes_log_format_error(logger, "%s\n", message);
    qes_log_format_fatal(logger, "%s\n", message);

    qes_logger_destroy(logger);
    return EXIT_SUCCESS;
}
