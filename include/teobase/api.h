/**
 * @file teobase/api.h
 * @brief API definition macroses for functions visibility.
 */

#pragma once

#ifndef TEOBASE_API_H
#define TEOBASE_API_H

#include "teobase/platform.h"

// Default behavior is static library.
// No defines are needed to build or use static library.
// Define TEOBASE_DYNAMIC and TEOBASE_EXPORTS to build dynamic library.
// Define TEOBASE_DYNAMIC to import dynamic library.
// Use TEOBASE_API macro on all public API functions.
// Use TEOBASE_INTERNAL macro on functions used only in this library.
#if defined(TEOBASE_DYNAMIC)
#if defined(TEONET_OS_WINDOWS)
#if defined(TEOBASE_EXPORTS)
#define TEOBASE_API __declspec(dllexport)
#else
#define TEOBASE_API __declspec(dllimport)
#endif
#define TEOBASE_INTERNAL
#else
#define TEOBASE_API __attribute__((visibility("default")))
#define TEOBASE_INTERNAL __attribute__((visibility("hidden")))
#endif
#else
#define TEOBASE_API
#define TEOBASE_INTERNAL
#endif

// This section is for Doxygen. Keep it in sync with macroses above.
#if defined(FORCE_DOXYGEN)
/// Define @TEOBASE_DYNAMIC when building or using teobase as dynamic library.
#define TEOBASE_DYNAMIC
#undef TEOBASE_DYNAMIC // We have to undefine all macroses to not screw up preprocessing.
/// Define @TEOBASE_EXPORTS when building teobase as dynamic library.
#define TEOBASE_EXPORTS
#undef TEOBASE_EXPORTS
/// This macro is used to declare public API functions.
#define TEOBASE_API
#undef TEOBASE_API
/// This macro is used to declare private API functions.
#define TEOBASE_INTERNAL
#undef TEOBASE_INTERNAL
#endif

#endif
