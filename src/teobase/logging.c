#include "teobase/logging.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_ANDROID)
#include <android/log.h>
#elif defined(TEONET_OS_WINDOWS)
#include "teobase/windows.h"
#endif

#include <stdio.h>  // snprintf, vsnprintf, NULL, size_t
#include <stdarg.h> // va_start, va_end, va_copy
#include <stdlib.h> // malloc, free

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

void teolog_output_compact(const char * file, int line, const char * func,
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
    printf("%s:%d '%s'>> [%s%s] %s\n", file, (int)line, func, tag, suffix, message);
#endif
}

static teologOutputFunction_t log_message = teolog_output_compact;

void set_log_output_function(teologOutputFunction_t logger) {
    log_message = logger;
}

void log_format(const char *file, int line, const char *func,
                TeoLogMessageType type, const char *tag, const char *fmt, ...) {
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

    log_message(file, line, func, type, tag, message);
    free(message);
}

void log_debug(const char *tag, const char *message) {
    if (log_message == NULL) {
        return;
    }

    log_message(NULL, -1, NULL, TEOLOG_SEVERITY_DEBUG, tag, message);
}

void log_info(const char* tag, const char* message) {
    if (log_message == NULL) {
        return;
    }

    log_message(NULL, -1, NULL, TEOLOG_SEVERITY_INFO, tag, message);
}

void log_warning(const char* tag, const char* message) {
    if (log_message == NULL) {
        return;
    }

    log_message(NULL, -1, NULL, TEOLOG_SEVERITY_IMPORTANT, tag, message);
}

void log_important(const char* tag, const char* message){
    if (log_message == NULL) {
        return;
    }

    log_message(NULL, -1, NULL, TEOLOG_SEVERITY_IMPORTANT, tag, message);
}

void log_error(const char* tag, const char* message) {
    if (log_message == NULL) {
        return;
    }

    log_message(NULL, -1, NULL, TEOLOG_SEVERITY_ERROR, tag, message);
}

