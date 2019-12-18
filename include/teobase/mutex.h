/**
 * @file teobase/mutex.h
 * @brief Cross-platform wrappers for mutex functions.
 */

#pragma once

#ifndef TEOBASE_MUTEX_H
#define TEOBASE_MUTEX_H

#include "teobase/platform.h"

#include <stdbool.h>

#if defined(TEONET_OS_WINDOWS)
#include "teobase/windows.h"
#else
#include <pthread.h>
#endif

#include "teobase/api.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Wrapper structure type for native mutex object. Do not use fields directly.
typedef struct teonetMutex {
#if defined(TEONET_OS_WINDOWS)
    CRITICAL_SECTION critical_section;
#else
    pthread_mutex_t mutex;
#endif
} teonetMutex;

/**
 * Initialize mutex object.
 *
 * @param mutex Pointer to uninitialized @teonetMutex structure.
 */
TEOBASE_API void teomutexInitialize(teonetMutex* mutex);

/**
 * Locks mutex object. Blocks calling thread if mutex object is currently locked.
 *
 * @note Attempt to gain recursive lock will cause a deadlock.
 *
 * @param mutex Pointer to @teonetMutex structure initialized using @teomutexInitialize.
 */
TEOBASE_API void teomutexLock(teonetMutex* mutex);

/**
 * Tries to lock mutex object. If mutex object is currently locked, returns immediately.
 *
 * @param mutex Pointer to @teonetMutex structure initialized using @teomutexInitialize.
 *
 * @return true if mutex was locked, false otherwise.
 */
TEOBASE_API bool teomutexTryLock(teonetMutex* mutex);

/**
 * Unlocks mutex object locked using @teomutexLock.
 *
 * @param mutex Pointer to @teonetMutex structure initialized using @teomutexInitialize.
 */
TEOBASE_API void teomutexUnlock(teonetMutex* mutex);

/**
 * Destroys mutex object created using @teomutexInitialize.
 *
 * @param mutex Pointer to @teonetMutex structure.
 */
TEOBASE_API void teomutexDestroy(teonetMutex* mutex);

#ifdef __cplusplus
}
#endif

#endif
