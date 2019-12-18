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
 * Initialize recursive lockable mutex object.
 *
 * @param mutex Pointer to uninitialized @a teonetMutex structure.
 */
TEOBASE_API void teomutexInitialize(teonetMutex* mutex);

/**
 * Locks mutex object. Blocks calling thread if mutex object is currently locked.
 *
 * @param mutex Pointer to @a teonetMutex structure initialized using @a teomutexInitialize.
 */
TEOBASE_API void teomutexLock(teonetMutex* mutex);

/**
 * Tries to lock mutex object. If mutex object is currently locked, returns immediately.
 *
 * @param mutex Pointer to @a teonetMutex structure initialized using @a teomutexInitialize.
 *
 * @return true if mutex was locked, false otherwise.
 */
TEOBASE_API bool teomutexTryLock(teonetMutex* mutex);

/**
 * Unlocks mutex object locked using @a teomutexLock.
 *
 * @param mutex Pointer to @a teonetMutex structure initialized using @a teomutexInitialize.
 */
TEOBASE_API void teomutexUnlock(teonetMutex* mutex);

/**
 * Destroys mutex object created using @a teomutexInitialize.
 *
 * @param mutex Pointer to @a teonetMutex structure.
 */
TEOBASE_API void teomutexDestroy(teonetMutex* mutex);

#ifdef __cplusplus
}
#endif

#endif
