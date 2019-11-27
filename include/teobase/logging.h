/**
 * @file teobase/logging.h
 * @brief Logging functions for user interaction and debugging.
 */

#ifndef TEOBASE_LOGGING_H
#define TEOBASE_LOGGING_H

/**
 * Message importance/verbocity type. Passed unmodified to output function.
 * Default loggers always output all messages of types (ERROR, IMPORTANT, INFO)
 * Debug builds additionally allows DEBUG messages.
 * All types CUSTOM and above are skipped by default
*/
typedef enum TeoLogMessageType {
  //! Error condition, possibly leaving program in inconsistent state.
  TEOLOG_SEVERITY_ERROR = 0,
  //! Significant message, like *_ERROR but leaves program consistent (or recoverable)
  TEOLOG_SEVERITY_IMPORTANT = 1,
  //! Ordinary message, states program execution stages
  TEOLOG_SEVERITY_INFO = 2,
  //! Extra verbosity, messages assisting in debugging, barely useful in normal execution
  TEOLOG_SEVERITY_DEBUG = 3,
  //! Application-defined types, intended for application-defined loggers
  TEOLOG_SEVERITY_CUSTOM = 4,
} TeoLogMessageType;

/**
 * Custom log output function declaration
 * Can be used to override default logger
*/
typedef void (*teologOutputFunction_t)(const char *file, int line,
                                       const char *func,
                                       TeoLogMessageType type, const char *tag,
                                       const char *message);

/**
 * Set current output function to @a logger.
 * If @a logger is NULL - disables logging
*/
void set_log_output_function(teologOutputFunction_t logger);

/**
 * Default output function. Produce something like
 * ./src/myFile.cpp:34899 ‘update‘>> [MyTagName|ERR] Kinda log example
*/
void teolog_output_default(const char *file, int line, const char *func,
                         TeoLogMessageType type, const char *tag,
                         const char *message);

/**
 * Compact output function. Produce something like
 * [MyTagName|ERR] Kinda log example
*/
void teolog_output_compact(const char *file, int line, const char *func,
                         TeoLogMessageType type, const char *tag,
                         const char *message);

void log_debug(const char* tag, const char* message);
void log_info(const char* tag, const char* message);
void log_warning(const char* tag, const char* message); //< log_important alias
void log_important(const char* tag, const char* message);
void log_error(const char* tag, const char* message);

void log_format(const char *file, int line, const char *func,
                TeoLogMessageType type, const char *tag, const char *fmt, ...);

#endif
