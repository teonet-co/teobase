/**
 * @file teobase/time.h
 * @brief Utility functions for working with time.
 */

#pragma once

#ifndef TEOBASE_TIME_H
#define TEOBASE_TIME_H

#include "teobase/types.h"

#include "teobase/api.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Unnamed enumeration with integer constants.
enum {
    MILLISECONDS_IN_SECOND = 1000,  ///< Amount of milliseconds in second.
    MICROSECONDS_IN_SECOND = 1000000,  ///< Amount of microseconds in second.
    MICROSECONDS_IN_MILLISECOND = 1000,  ///< Amount of microseconds in millisecond.
};

/**
 * Get current time in microseconds.
 *
 * @return current time in microseconds since Unix Epoch.
 *
 * @note On 32-bit linux systems return value is limited to maximum value of 32-bit signed integer.
 */

TEOBASE_API int64_t teotimeGetCurrentTimeUs(void);

/**
 * Get current time in milliseconds.
 *
 * @return current time in milliseconds since Unix Epoch.
 *
 * @note On 32-bit linux systems return value is limited to maximum value of 32-bit signed integer.
 */

TEOBASE_API int64_t teotimeGetCurrentTimeMs(void);

/**
 * Get time in microseconds between saved moment of time and current time.
 *
 * @param time_value_us Saved moment of time in microseconds.
 *
 * @return Time in microseconds between time_value and current time.
 *
 * @note Return value can be negative if time_value is in the future.
 */

TEOBASE_API int64_t teotimeGetTimePassedUs(int64_t time_value_us);

/**
 * Get time in milliseconds between saved moment of time and current time.
 *
 * @param time_value_ms Saved moment of time in milliseconds.
 *
 * @return Time in milliseconds between time_value and current time.
 *
 * @note Return value can be negative if time_value is in the future.
 */
TEOBASE_API int64_t teotimeGetTimePassedMs(int64_t time_value_ms);

#ifdef __cplusplus
}
#endif

#endif
