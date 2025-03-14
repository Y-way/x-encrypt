#pragma once

#include "platforms.h"

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

#if !defined(X_ENCRYPT_EXPORT)
#ifdef _MSC_VER
# include <malloc.h>
# define X_ENCRYPT_EXPORT __declspec(dllexport)
#elif X_ENCRYPT_TARGET_PSP2 || X_ENCRYPT_TARGET_PS4
# define X_ENCRYPT_EXPORT __declspec(dllexport)
#else
# define X_ENCRYPT_EXPORT __attribute__ ((visibility ("default")))
#endif
#endif

#if !defined(X_ENCRYPT_IMPORT)
#ifdef _MSC_VER
# include <malloc.h>
# define X_ENCRYPT_IMPORT __declspec(dllimport)
#elif X_ENCRYPT_TARGET_PSP2 || X_ENCRYPT_TARGET_PS4
# define X_ENCRYPT_IMPORT __declspec(dllimport)
#else
# define X_ENCRYPT_IMPORT
#endif
#endif

#ifdef XENCRYPT_EXPORT_API
# define XENCRYPT_API X_ENCRYPT_EXPORT
#elif XENCRYPT_IMPORT_API
# define XENCRYPT_API X_ENCRYPT_IMPORT
#else
# define XENCRYPT_API
#endif