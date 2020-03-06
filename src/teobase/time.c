#include "teobase/time.h"

#include <string.h>

#include "teobase/types.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_WINDOWS)
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

// Get current time in microseconds.
int64_t teotimeGetCurrentTimeUs() {
    int64_t current_time_us;

#if defined(TEONET_OS_WINDOWS)
    struct __timeb64 time_value;
    memset(&time_value, 0, sizeof(time_value));

    _ftime64_s(&time_value);

    current_time_us = time_value.time * MICROSECONDS_IN_SECOND + (int64_t)time_value.millitm * MICROSECONDS_IN_MILLISECOND;
#else
    struct timeval time_value;
    memset(&time_value, 0, sizeof(time_value));

    gettimeofday(&time_value, 0);

    // Cast to int64_t is needed on 32-bit Unix systems.
    current_time_us = (int64_t)time_value.tv_sec * MICROSECONDS_IN_SECOND + time_value.tv_usec;
#endif

    return current_time_us;
}

// Get current time in milliseconds.
int64_t teotimeGetCurrentTimeMs() {
    int64_t current_time_ms;

#if defined(TEONET_OS_WINDOWS)
    struct __timeb64 time_value;
    memset(&time_value, 0, sizeof(time_value));

    _ftime64_s(&time_value);

    current_time_ms = time_value.time * MILLISECONDS_IN_SECOND + time_value.millitm;
#else
    struct timeval time_value;
    memset(&time_value, 0, sizeof(time_value));

    gettimeofday(&time_value, 0);

    // Cast to int64_t is needed on 32-bit Unix systems.
    current_time_ms = (int64_t)time_value.tv_sec * MILLISECONDS_IN_SECOND + time_value.tv_usec / MICROSECONDS_IN_MILLISECOND;
#endif

    return current_time_ms;
}

// Get amount of time in microseconds between saved moment of time and current time.
int64_t teotimeGetTimePassedUs(int64_t time_value_us) {
    int64_t current_time_us = teotimeGetCurrentTimeUs();

    return current_time_us - time_value_us;
}

// Get amount of time in milliseconds between saved moment of time and current time.
int64_t teotimeGetTimePassedMs(int64_t time_value_ms) {
    int64_t current_time_ms = teotimeGetCurrentTimeMs();

    return current_time_ms - time_value_ms;
}
