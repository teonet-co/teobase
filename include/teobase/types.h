/**
 * @file teobase/types.h
 * @brief Cross-platform include file for basic types.
 */

#pragma once

#ifndef TEOBASE_TYPES_H
#define TEOBASE_TYPES_H

// bool
#include <stdbool.h>

// size_t, offsetof, NULL
#include <stddef.h>

// intx_t, uintx_t, intptr_t
#include <stdint.h>

// type_MIN, type_MAX
#include <limits.h>

#include "teobase/platform.h"

// ssize_t Posix type.
#if defined(TEONET_OS_WINDOWS)
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#else
#include <sys/types.h>
#endif

#endif
