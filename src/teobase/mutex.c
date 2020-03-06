#include "teobase/mutex.h"

#include "teobase/types.h"

#include "teobase/platform.h"

#if defined(TEONET_OS_WINDOWS)
#include "teobase/windows.h"
#else
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#endif

#include "teobase/logging.h"

#if !defined(TEONET_OS_WINDOWS)
static pthread_once_t mutex_attributes_once = PTHREAD_ONCE_INIT;

static pthread_mutexattr_t mutex_attributes;

static void teomutexInitializeAttributes() {
    pthread_mutexattr_init(&mutex_attributes);
    pthread_mutexattr_settype(&mutex_attributes, PTHREAD_MUTEX_RECURSIVE);
}
#endif

// Initialize mutex object.
void teomutexInitialize(teonetMutex* mutex) {
#if defined(TEONET_OS_WINDOWS)
    InitializeCriticalSection(&mutex->critical_section);
#else
    pthread_once(&mutex_attributes_once, teomutexInitializeAttributes);

    int init_result = pthread_mutex_init(&mutex->mutex, &mutex_attributes);

    if (init_result != 0) {
        LTRACK_E("TeoBase", "Failed to initialize mutex. Error code: %d.", init_result);
        abort();
    }
#endif
}

// Locks mutex object.
void teomutexLock(teonetMutex* mutex) {
#if defined(TEONET_OS_WINDOWS)
    EnterCriticalSection(&mutex->critical_section);
#else
    int lock_result = pthread_mutex_lock(&mutex->mutex);

    if (lock_result != 0) {
        LTRACK_E("TeoBase", "Failed to lock mutex. Error code: %d.", lock_result);
        abort();
    }
#endif
}

// Tries to lock mutex object.
bool teomutexTryLock(teonetMutex* mutex) {
#if defined(TEONET_OS_WINDOWS)
    BOOL try_lock_result = TryEnterCriticalSection(&mutex->critical_section);

    return try_lock_result != 0 ? true : false;
#else
    int try_lock_result = pthread_mutex_trylock(&mutex->mutex);

    if (try_lock_result == 0) {
        return true;
    } else if (try_lock_result == EBUSY) {
        return false;
    } else {
        LTRACK_E("TeoBase", "Error while trying to lock mutex. Error code: %d.", try_lock_result);
        abort();
    }
#endif
}

// Unlocks locked mutex object.
void teomutexUnlock(teonetMutex* mutex) {
#if defined(TEONET_OS_WINDOWS)
    LeaveCriticalSection(&mutex->critical_section);
#else
    int unlock_result = pthread_mutex_unlock(&mutex->mutex);

    if (unlock_result != 0) {
        LTRACK_E("TeoBase", "Failed to unlock mutex. Error code: %d.", unlock_result);
        abort();
    }
#endif
}

// Destroys mutex object.
void teomutexDestroy(teonetMutex* mutex) {
#if defined(TEONET_OS_WINDOWS)
    DeleteCriticalSection(&mutex->critical_section);
#else
    int destroy_result = pthread_mutex_destroy(&mutex->mutex);

    if (destroy_result != 0) {
        LTRACK_E("TeoBase", "Failed to destroy mutex. Error code: %d.", destroy_result);
        abort();
    }
#endif
}
