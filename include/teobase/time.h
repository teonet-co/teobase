/**
 * @file teobase/time.h
 * @brief Utility functions for working with time.
 */

#ifndef TEOBASE_TIME_H
#define TEOBASE_TIME_H

#include <stdint.h>

/**
 * Get current time in milliseconds.
 *
 * @return current time in milliseconds since Unix Epoch.
 *
 * @note On 32-bit linux systems return value is limited to maximum value of 32-bit signed integer.
 */

int64_t teotimeGetCurrentTimeMs();

/**
 * Get time in milliseconds between saved moment of time and current time.
 *
 * @param time_value Saved moment of time.
 *
 * @return Time in milliseconds between time_value and current time.
 *
 * @note Return value can be negative if time_value is in the future.
 */
int64_t teotimeGetTimePassedMs(int64_t time_value_ms);

#endif
