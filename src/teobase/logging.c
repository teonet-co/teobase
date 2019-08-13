#include "teobase/logging.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_ANDROID)
#include <android/log.h>
#elif defined(TEONET_OS_WINDOWS)
#include "teobase/windows.h"
#else
#include <stdio.h>
#endif

static inline void log_message(const char* tag, const char* message) {
#if defined(TEONET_OS_ANDROID)
    __android_log_print(ANDROID_LOG_ERROR, tag, "%s", message);
#elif defined(TEONET_OS_WINDOWS)
    OutputDebugStringA(message);
#else
    printf("[%s] %s\n", tag, message);
#endif
}

void log_debug(const char* tag, const char* message) {
    log_message(tag, message);
}

void log_info(const char* tag, const char* message) {
    log_message(tag, message);
}

void log_warning(const char* tag, const char* message) {
    log_message(tag, message);
}

void log_error(const char* tag, const char* message) {
    log_message(tag, message);
}
