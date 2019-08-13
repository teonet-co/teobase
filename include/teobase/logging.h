/**
 * @file teobase/logging.h
 * @brief Logging functions for user interaction and debugging.
 */

#ifndef TEOBASE_LOGGING_H
#define TEOBASE_LOGGING_H

void log_debug(const char* tag, const char* message);
void log_info(const char* tag, const char* message);
void log_warning(const char* tag, const char* message);
void log_error(const char* tag, const char* message);

#endif
