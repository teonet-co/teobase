#include "teobase/logging.h"

#include <stdarg.h> // va_start, va_end, va_copy
#include <stdio.h>  // snprintf, vsnprintf, NULL, size_t
#include <stdlib.h> // malloc, free

#include "teobase/types.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_ANDROID)
#include <android/log.h>
#elif defined(TEONET_OS_WINDOWS)
#include "teobase/windows.h"
#endif

static inline const char *log_suffix(TeoLogMessageType value) {
    switch (value) {
    case TEOLOG_SEVERITY_ERROR: return ":ERR";
    case TEOLOG_SEVERITY_IMPORTANT: return ":IMP";
    case TEOLOG_SEVERITY_INFO: return ":INF";
    case TEOLOG_SEVERITY_DEBUG: return ":DBG";
    default: break;
    }
    return "";
}

#if defined(TEONET_OS_ANDROID)
static inline android_LogPriority log_prio(TeoLogMessageType value) {
    switch (value) {
    case TEOLOG_SEVERITY_ERROR: return ANDROID_LOG_ERROR;
    case TEOLOG_SEVERITY_IMPORTANT: return ANDROID_LOG_WARN;
    case TEOLOG_SEVERITY_INFO: return ANDROID_LOG_INFO;
    case TEOLOG_SEVERITY_DEBUG: return ANDROID_LOG_DEBUG;
    default: break;
    }
    return ANDROID_LOG_VERBOSE;
}
#endif

#ifdef DEBUG
static const TeoLogMessageType outputLevel = TEOLOG_SEVERITY_DEBUG;
#else
static const TeoLogMessageType outputLevel = TEOLOG_SEVERITY_INFO;
#endif

void teolog_output_compact(const char *file, int line, const char *func,
                           TeoLogMessageType type, const char *tag,
                           const char *message) {
    if (type > outputLevel) {
        return; //  verbosity limit
    }
    if (message == NULL) { message = "<NULL>"; }
    if (tag == NULL) { tag = ""; }

#if defined(TEONET_OS_ANDROID)
    __android_log_print(log_prio(type), tag, "%s", message);
#else
    const char *suffix = log_suffix(type);
    printf("[%s%s] %s\n", tag, suffix, message);
#endif
}

void teolog_output_default(const char *file, int line, const char *func,
                           TeoLogMessageType type, const char *tag,
                           const char *message) {
    if (type > outputLevel) {
        return; //  verbosity limit
    }
    if (message == NULL) { message = "<NULL>"; }
    if (tag == NULL) { tag = ""; }
    if (file == NULL) { file = "??"; }
    if (func == NULL) { func = "??"; }

#if defined(TEONET_OS_ANDROID)
    __android_log_print(log_prio(type), tag, "%s:%d '%s'>> %s", file, (int)line,
                        func, message);
#else
    const char *suffix = log_suffix(type);
    printf("%s:%d '%s'>> [%s%s] %s\n", file, (int)line, func, tag, suffix,
           message);
#endif
}

static teologOutputFunction_t log_message = teolog_output_compact;

void set_log_output_function(teologOutputFunction_t logger) {
    log_message = logger;
}

static inline void invoke_log_callback(const char *file, int line,
                                       const char *func, TeoLogMessageType type,
                                       const char *tag, const char *message) {
    // Callback variable can be changed in another thread. Local copy
    // guarantees that it was not changed between null check and invocation.
    teologOutputFunction_t log_message_copy = log_message;

    if (log_message_copy != NULL) {
        log_message_copy(file, line, func, type, tag, message);
    }
}

void log_format(const char *file, int line, const char *func,
                TeoLogMessageType type, const char *tag, const char *fmt, ...) {
    // log_message callback will be checked for NULL in invoke_log_callback.
    // This additional check allow skip unnecessary string formatting.
    if (log_message == NULL) { return; }

    va_list args_noop;
    va_start(args_noop, fmt);
    int message_len = vsnprintf(NULL, 0, fmt, args_noop);
    va_end(args_noop);

    if (message_len < 1) { return; }
    size_t buffer_length = (size_t)message_len + 1;
    char *message = (char *)malloc(buffer_length);
    if (message == NULL) { return; }

    va_list args_fmt;
    va_start(args_fmt, fmt);
    vsnprintf(message, buffer_length, fmt, args_fmt);
    va_end(args_fmt);

    invoke_log_callback(file, line, func, type, tag, message);
    free(message);
}

void log_debug(const char *tag, const char *message) {
    invoke_log_callback(NULL, -1, NULL, TEOLOG_SEVERITY_DEBUG, tag, message);
}

void log_info(const char *tag, const char *message) {
    invoke_log_callback(NULL, -1, NULL, TEOLOG_SEVERITY_INFO, tag, message);
}

void log_warning(const char *tag, const char *message) {
    invoke_log_callback(NULL, -1, NULL, TEOLOG_SEVERITY_IMPORTANT, tag,
                        message);
}

void log_important(const char *tag, const char *message) {
    invoke_log_callback(NULL, -1, NULL, TEOLOG_SEVERITY_IMPORTANT, tag,
                        message);
}

void log_error(const char *tag, const char *message) {
    invoke_log_callback(NULL, -1, NULL, TEOLOG_SEVERITY_ERROR, tag, message);
}

// Prints given data to a buffer.
void dump_bytes(char* buffer, int buffer_len, const uint8_t* data, int data_len) {
    // Target character buffer must be valid.
    if (buffer == NULL || buffer_len < 1) {
        return;
    }

    buffer[0] = 0; // for case of early exit

    if (data == NULL) {
        return;
    }

    while (data_len > 0 && buffer_len > 4) {
#if defined(TEONET_COMPILER_MSVC)
        sprintf_s(buffer, buffer_len, "%02X ", *data);
#else
        sprintf(buffer, "%02X ", *data);
#endif
        ++data;
        --data_len;
        buffer += 3;
        buffer_len -= 3;
    }
}
