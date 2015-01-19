/*
 * ============================================================================
 *
 *       Filename:  qes_log.c
 *
 *    Description:  Logging module of libngs2
 *        License:  GPLv3+
 *         Author:  Kevin Murray, spam@kdmurray.id.au
 *
 * ============================================================================
 */

#include "qes_log.h"


struct qes_logger *
qes_logger_create(void)
{
    return qes_calloc(1, sizeof(struct qes_logger));
}

int
qes_logger_init(struct qes_logger  *logger,
                const char         *name,
                enum qes_log_level  level)
{
    if (logger == NULL) return 1;

    if (name != NULL) {
        logger->name = strdup(name);
    }
    logger->level = level;
    return 0;
}

int
qes_logger_add_destination_struct(struct qes_logger            *logger,
                                  struct qes_log_destination   *destination)
{
    struct qes_log_destination *new = NULL;
    size_t new_sz = logger->n_destinations + 1;

    new = qes_realloc(logger->destinations,
                      new_sz * sizeof(*logger->destinations));
    if (new == NULL) {
        return 1;
    }
    memcpy(&new[new_sz - 1], destination, sizeof(*new));
    logger->destinations = new;
    logger->n_destinations = new_sz;
    return 0;
}

int
qes_logger_add_destination(struct qes_logger   *logger,
                           FILE                *stream,
                           enum qes_log_level   level)
{
    struct qes_log_destination *new = NULL;
    size_t new_sz = logger->n_destinations + 1;

    new = qes_realloc(logger->destinations,
                      new_sz * sizeof(*logger->destinations));
    if (new == NULL) {
        return 1;
    }
    logger->destinations = new;
    logger->n_destinations = new_sz;
    /* For ease of refernce below, save the ptr to the unallocated final
     * struct*/
    new = &new[new_sz - 1];
    new->stream = stream;
    new->level = level;
    return 0;
}

void
_qes_logger_destroy(struct qes_logger  *logger)
{
    if (logger != NULL) {
        qes_free(logger->name);
        qes_free(logger->destinations);
        qes_free(logger);
    }
}

struct qes_log_entry *
qes_log_entry_create(void)
{
    return qes_calloc(1, sizeof(struct qes_log_entry));
}

int
qes_log_entry_init(struct qes_log_entry *entry, enum qes_log_level level,
                   const char *message)
{
    if (entry == NULL || message == NULL) return -1;

    entry->level = level;
    entry->message = strdup(message);
    return 0;
}

struct qes_log_entry *
qes_log_entry_format_va(enum qes_log_level  level,
                        const char         *format,
                        va_list             args)
{
    int res = 0;
    char *message = NULL;
    struct qes_log_entry *entry;


    /* Format the error message w/ user input */
    res = vasprintf(&message, format, args);
    if (res < 1) {
        /* Alloc inside vasprintf failed */
        return NULL;
    }
    /* Make the entry struct */
    entry = qes_log_entry_create();
    res = qes_log_entry_init(entry, level, message);
    if (res != 0) {
        /* Init failed */
        qes_log_entry_destroy(entry);
        return NULL;
    }
    return entry;
}

struct qes_log_entry *
qes_log_entry_format(enum qes_log_level     level,
                     const char            *format,
                     ...)
{
    va_list args;
    struct qes_log_entry *entry;


    /* Format the error message w/ user input */
    va_start(args, format);
    entry = qes_log_entry_format_va(level, format, args);
    va_end(args);
    return entry;
}

void
_qes_log_entry_destroy(struct qes_log_entry    *entry)
{
    if (entry != NULL) {
        qes_free(entry->message);
        qes_free(entry);
    }
}

static int
_qes_logger_write_entry(struct qes_logger      *logger,
                        struct qes_log_entry   *entry)
{
    size_t iii;
    int res;

    if (logger == NULL || entry == NULL) return 1;

    /* Message is to unimportant for this logger */
    if (logger->level > entry->level) return 0;

    for (iii = 0; iii < logger->n_destinations; iii++) {
        struct qes_log_destination *dest = &logger->destinations[iii];

        /* Message is to unimportant for this destination */
        if (dest->level > entry->level) continue;

        res = fprintf(dest->stream, "%s", entry->message);
        fflush(dest->stream);
        if (res < 0) return 1;
    }
    return 0;
}

int
qes_log_message(struct qes_logger      *logger,
                enum qes_log_level      level,
                const char             *message)
{
    struct qes_log_entry *entry;

    entry = qes_log_entry_format(level, "%s", message);
    return _qes_logger_write_entry(logger, entry);
}

int
qes_log_format(struct qes_logger       *logger,
               enum qes_log_level       level,
               const char              *format,
               ...)
{
    struct qes_log_entry *entry;
    va_list args;

    va_start(args, format);
    entry = qes_log_entry_format_va(level, format, args);
    va_end(args);
    return _qes_logger_write_entry(logger, entry);
}
