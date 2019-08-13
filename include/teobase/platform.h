/**
 * @file teobase/platform.h
 * @brief Defines for platform dependent and compiler dependent code.
 */

#ifndef TEOBASE_PLATFORM_H
#define TEOBASE_PLATFORM_H

// Determine target operating system.
#if defined(__ANDROID__)
// Defined if target OS is Android.
#define TEONET_OS_ANDROID
#elif defined(__linux__)
// Defined if target OS is Linux.
#define TEONET_OS_LINUX
#elif defined(_WIN32)
// Defined if target OS is Windows.
#define TEONET_OS_WINDOWS
#elif defined(__APPLE__)
// This file is needed to be able to check for macOS or iOS.
#include <TargetConditionals.h>
// Macro TARGET_OS_IPHONE can be defined to 0, so we also need to check it's value.
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
// Defined if target OS is iOS.
#define TEONET_OS_IOS
#else
// Defined if target OS is macOS.
#define TEONET_OS_MACOS
#endif
#else
#error Unsupported target OS.
#endif

// Determine current compiler type.
#if defined(__MINGW32__)
// Defined if current compiler is MINGW. Use this for compiler-dependent code.
#define TEONET_COMPILER_MINGW
#elif defined(__GNUC__)
// Defined if current compiler is GCC. Use this for compiler-dependent code.
#define TEONET_COMPILER_GCC
#elif defined(_MSC_VER)
// Defined if current compiler is MSVC. Use this for compiler-dependent code.
#define TEONET_COMPILER_MSVC
#else
#error Unsupported compiler.
#endif

// This section is for doxygen. Keep it in sync with macroses above.
#if defined(FORCE_DOXYGEN)
// Defined if target OS is Android.
#define TEONET_OS_ANDROID
#undef TEONET_OS_ANDROID  // We have to undefine all macroses to not screw up preprocessing.
/// Defined if target OS is Linux.
#define TEONET_OS_LINUX
#undef TEONET_OS_LINUX
/// Defined if target OS is Windows.
#define TEONET_OS_WINDOWS
#undef TEONET_OS_WINDOWS
/// Defined if target OS is iOS.
#define TEONET_OS_IOS
#undef TEONET_OS_IOS
/// Defined if target OS is macOS.
#define TEONET_OS_MACOS
#undef TEONET_OS_MACOS

// Defined if current compiler is MINGW. Use this for compiler-dependent code.
#define TEONET_COMPILER_MINGW
#undef TEONET_COMPILER_MINGW
/// Defined if current compiler is GCC. Use this for compiler-dependent code.
#define TEONET_COMPILER_GCC
#undef TEONET_COMPILER_GCC
/// Defined if current compiler is MSVC. Use this for compiler-dependent code.
#define TEONET_COMPILER_MSVC
#undef TEONET_COMPILER_MSVC
#endif

#endif
