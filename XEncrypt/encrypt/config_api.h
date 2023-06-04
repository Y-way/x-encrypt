#pragma once

#include "config_platforms.h"
#include "config_api_platforms.h"
#include "api_types.h"

#if X_ENCRYPT_TARGET_ARMV7
// On ARMv7 with Thumb instructions the lowest bit is always set.
// With Thumb2 the second-to-lowest bit is also set. Mask both of
// them off so that we can do a comparison properly based on the data
// from the linker map file.
#define X_ENCRYPT_POINTER_SPARE_BITS 3
#else
// Some compilers align functions by default (MSVC), some do not (GCC).
// Do not mask bits on platforms that do not absolutely require it.
#define X_ENCRYPT_POINTER_SPARE_BITS 0
#endif

#if X_ENCRYPT_COMPILER_MSVC || defined(__ARMCC_VERSION)
#define NORETURN __declspec(noreturn)
#elif (X_ENCRYPT_POINTER_SPARE_BITS == 0) && (defined(__clang__) || defined(__GNUC__))
#define NORETURN __attribute__ ((noreturn))
#else
#define NORETURN
#endif

#if X_ENCRYPT_TARGET_IOS || X_ENCRYPT_TARGET_ANDROID || X_ENCRYPT_TARGET_DARWIN
#define REAL_NORETURN __attribute__ ((noreturn))
#else
#define REAL_NORETURN NORETURN
#endif
