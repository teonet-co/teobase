/**
 * @file teobase/logging.h
 * @brief Logging functions for user interaction and debugging.
 */

#pragma once

#ifndef TEOBASE_LOGGING_H
#define TEOBASE_LOGGING_H

#include "teobase/types.h"

#include "teobase/api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Message importance/verbosity type. Passed unmodified to output function.
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
TEOBASE_API void set_log_output_function(teologOutputFunction_t logger);

/**
 * Default output function. Produce something like
 * ./src/myFile.cpp:34899 ‘update‘>> [MyTagName|ERR] Kinda log example
*/
TEOBASE_API void teolog_output_default(const char *file, int line, const char *func,
                                       TeoLogMessageType type, const char *tag,
                                       const char *message);

/**
 * Compact output function. Produce something like
 * [MyTagName|ERR] Kinda log example
*/
TEOBASE_API void teolog_output_compact(const char *file, int line, const char *func,
                                       TeoLogMessageType type, const char *tag,
                                       const char *message);

TEOBASE_API void log_debug(const char* tag, const char* message);
TEOBASE_API void log_info(const char* tag, const char* message);
TEOBASE_API void log_warning(const char* tag, const char* message); // Alias for log_important
TEOBASE_API void log_important(const char* tag, const char* message);
TEOBASE_API void log_error(const char* tag, const char* message);

TEOBASE_API void log_format(const char *file, int line, const char *func,
                            TeoLogMessageType type, const char *tag, const char *fmt, ...);

/**
 * Line track - log message along with file/line/function name
 * use it like
 * LTRACK_E("subSysTag", "Received nullptr from %s:%d\n\t\tAborting",
 *          peername, (int)port);
*/
#define LTRACK(tag, ...)                                                       \
  log_format(__FILE__, __LINE__, __FUNCTION__, TEOLOG_SEVERITY_DEBUG,          \
             tag, __VA_ARGS__)

#define LTRACK_E(tag, ...)                                                     \
  log_format(__FILE__, __LINE__, __FUNCTION__, TEOLOG_SEVERITY_ERROR,          \
             tag, __VA_ARGS__)

#define LTRACK_I(tag, ...)                                                     \
  log_format(__FILE__, __LINE__, __FUNCTION__, TEOLOG_SEVERITY_INFO,           \
             tag, __VA_ARGS__)

/**
 * Conditional line track
 * if @a COND is truthy value then does same as LTRACK, otherwise - noop
*/
#define CLTRACK(COND, tag, ...)                                                \
  ((COND) ? log_format(__FILE__, __LINE__, __FUNCTION__,                       \
                       TEOLOG_SEVERITY_DEBUG, tag, __VA_ARGS__)                \
          : (void)0)

#define CLTRACK_E(COND, tag, ...)                                              \
  ((COND) ? log_format(__FILE__, __LINE__, __FUNCTION__,                       \
                       TEOLOG_SEVERITY_ERROR, tag, __VA_ARGS__)                \
          : (void)0)

#define CLTRACK_I(COND, tag, ...)                                              \
  ((COND) ? log_format(__FILE__, __LINE__, __FUNCTION__,                       \
                       TEOLOG_SEVERITY_INFO, tag, __VA_ARGS__)                 \
          : (void)0)

/**
 * Prints given @a data to @a buffer in form "XX XX XX XX ".
 *
 * Prints byte-by-byte, and fills @a buffer via 3 characters at a time
 * until buffer space is sufficent, up to data_len always appends
 * terminating NUL character.
 *
 * @param buffer Destination buffer, non-null pointer to memory region of @a
 * buffer_len bytes
 * @param buffer_len destination buffer length, must be atleast 1 byte (for NUL
 * character)
 * @param data source bytes to be dumped, non-null pointer of @a data_len bytes
 * @param data_len amount of bytes to be dumped, can be zero
 */
TEOBASE_API void dump_bytes(char* buffer, int buffer_len, const uint8_t* data, int data_len);

#ifdef __cplusplus
}
#endif

#endif
