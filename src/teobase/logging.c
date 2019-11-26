#include "teobase/logging.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_ANDROID)
#include <android/log.h>
#elif defined(TEONET_OS_WINDOWS)
#include "teobase/windows.h"
#else
#include <stdio.h>
#endif

static inline void default_log_message(const char* tag, const char* message) {
#if defined(TEONET_OS_ANDROID)
    __android_log_print(ANDROID_LOG_ERROR, tag, "%s", message);
#elif defined(TEONET_OS_WINDOWS)
    OutputDebugStringA(message);
#else
    printf("[%s] %s\n", tag, message);
#endif
}

static teologOutputFunction_t log_message = default_log_message;

void set_log_output_function(teologOutputFunction_t logger) {
    log_message = logger;
}

void set_default_log_output(void) {
    log_message = default_log_message;
}

void log_debug(const char *tag, const char *message) {
    if (log_message == NULL) {
        return;
    }

    log_message(tag, message);
}

void log_info(const char* tag, const char* message) {
    if (log_message == NULL) {
        return;
    }

    log_message(tag, message);
}

void log_warning(const char* tag, const char* message) {
    if (log_message == NULL) {
        return;
    }

    log_message(tag, message);
}

void log_error(const char* tag, const char* message) {
    if (log_message == NULL) {
        return;
    }

    log_message(tag, message);
}
