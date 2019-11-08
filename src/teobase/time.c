#include "teobase/time.h"

#include <stdint.h>
#include <string.h>

#include "teobase/platform.h"

#if defined(TEONET_OS_WINDOWS)
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

// Get current time in milliseconds.
int64_t teotimeGetCurrentTimeMs() {
    int64_t current_time_ms;

#if defined(TEONET_OS_WINDOWS)
    struct __timeb64 time_value;
    memset(&time_value, 0, sizeof(time_value));

    _ftime64_s(&time_value);

    current_time_ms = time_value.time * 1000 + time_value.millitm;
#else
    struct timeval time_value;
    memset(&time_value, 0, sizeof(time_value));

    gettimeofday(&time_value, 0);

    // Cast to int64_t is needed on 32-bit unix systems.
    current_time_ms = (int64_t)time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
#endif

    return current_time_ms;
}

// Get amount of time between saved moment of time and current time.
int64_t teotimeGetTimePassedMs(int64_t time_value_ms) {
    int64_t current_time_ms = teotimeGetCurrentTimeMs();

    return current_time_ms - time_value_ms;
}
