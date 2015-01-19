/*
 * ============================================================================
 *
 *       Filename:  qes_log.h
 *    Description:  Logging module of libngs2
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#ifndef QES_LOG_H
#define QES_LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <qes_util.h>

enum qes_log_level {
    /* The idea is that the user can add values inbetween these, if they need
     * to. a la Python's logging module. */
    QES_LOG_DEBUG = 0,
    QES_LOG_INFO = 10,
    QES_LOG_WARNING = 20,
    QES_LOG_ERROR = 30,
    QES_LOG_FATAL = 40,
};
#define QES_LOG_LEVEL_FIRST_USER 41

typedef enum qes_log_level QesLogLevel;

struct qes_log_destination {
    FILE *stream;
    enum qes_log_level level;
};
typedef struct qes_log_destination QesLogDestination;

struct qes_logger {
    struct qes_log_destination *destinations;
    size_t n_destinations;
    enum qes_log_level level;
    char *name;
    int lock;
};
typedef struct qes_logger QesLogger;

struct qes_log_entry {
    char *message;
    enum qes_log_level level;
};
typedef struct qes_log_entry QesLogEntry;


struct qes_logger *qes_logger_create(void);
int qes_logger_init(struct qes_logger *logger, const char *name,
                    enum qes_log_level level);
int qes_logger_add_destination_struct(struct qes_logger *logger,
                                      struct qes_log_destination *destination);
int qes_logger_add_destination(struct qes_logger *logger, FILE *stream,
                               enum qes_log_level level);
void _qes_logger_destroy(struct qes_logger *logger);
#define qes_logger_destroy(l) ({ _qes_logger_destroy(l); l = NULL; })


struct qes_log_entry *qes_log_entry_create(void);
int qes_log_entry_init(struct qes_log_entry *entry, enum qes_log_level level,
                       const char *message);
struct qes_log_entry *qes_log_entry_format(enum qes_log_level level,
                                           const char *format, ...);
struct qes_log_entry *qes_log_entry_format_va(enum qes_log_level level,
                                           const char *format, va_list args);
void _qes_log_entry_destroy(struct qes_log_entry *log_entry);
#define qes_log_entry_destroy(l) ({ _qes_log_entry_destroy(l); l = NULL; })


int qes_log_message(struct qes_logger *logger, enum qes_log_level level,
                    const char *message);
#ifndef NDEBUG
#define qes_log_message_debug(log, msg) qes_log_message(log, QES_LOG_DEBUG, msg)
#else
#define qes_log_message_debug(log, msg)
#endif
#define qes_log_message_info(log, msg) qes_log_message(log, QES_LOG_INFO, msg)
#define qes_log_message_error(log, msg) qes_log_message(log, QES_LOG_ERROR, msg)
#define qes_log_message_fatal(log, msg) qes_log_message(log, QES_LOG_FATAL, msg)


int qes_log_format(struct qes_logger *logger, enum qes_log_level level,
                   const char *format, ...);
#ifndef NDEBUG
#define qes_log_format_debug(log, fmt, ...) \
        qes_log_format(log, QES_LOG_DEBUG, fmt, __VA_ARGS__)
#else
#define qes_log_format_debug(log, fmt, ...)
#endif
#define qes_log_format_info(log, fmt, ...) \
        qes_log_format(log, QES_LOG_INFO, fmt, __VA_ARGS__)
#define qes_log_format_error(log, fmt, ...) \
        qes_log_format(log, QES_LOG_ERROR, fmt, __VA_ARGS__)
#define qes_log_format_fatal(log, fmt, ...) \
        qes_log_format(log, QES_LOG_FATAL, fmt, __VA_ARGS__)


#endif /* QES_LOG_H */
