/**
 * @file teobase/logging.h
 * @brief Logging functions for user interaction and debugging.
 */

#ifndef TEOBASE_LOGGING_H
#define TEOBASE_LOGGING_H

/**! Custom log output function declaration
 * Can be used to override default logger
 *  */
typedef void (*teologOutputFunction_t)(const char* tag, const char* message);

/**! Set current output function to @a logger.
 * If @a logger is NULL - disables logging
 *  */
void set_log_output_function(teologOutputFunction_t logger);

/**! Set current output function to default one
 * thus, undoing effect of set_log_output_function
 *  */
void set_default_log_output(void);

void log_debug(const char* tag, const char* message);
void log_info(const char* tag, const char* message);
void log_warning(const char* tag, const char* message);
void log_error(const char* tag, const char* message);

#endif
