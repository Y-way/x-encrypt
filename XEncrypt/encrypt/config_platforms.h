#pragma once

#include <stddef.h> // ptrdiff_t

#if defined(__aarch64__) && defined(__arm__)
#error We assume both __aarch64__ and __arm__ cannot be defined at tha same time.
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
#define X_ENCRYPT_TARGET_ARM64 1
#define X_ENCRYPT_TARGET_ARMV7 0
#elif defined(__arm__)
#define X_ENCRYPT_TARGET_ARM64 0
#define X_ENCRYPT_TARGET_ARMV7 1
#else
#define X_ENCRYPT_TARGET_ARM64 0
#define X_ENCRYPT_TARGET_ARMV7 0
#endif

// Large executables on ARM64 and ARMv7 can cause linker errors.
// Specifically, the arm instruction set limits the range a branch can
// take (e.g. 128MB on ARM64). Normally, the linker will insert branch
// islands to bridge gaps larger than the maximum branch range. However,
// branch islands only work within a section, not across sections. So if
// IL2CPP puts managed code into a specific section of the binary, branch
// isalnds won't work. That means that proejcts with a large executable
// size may fail to link.
//
// Set the define X_ENCRYPT_LARGE_EXECUTABLE_ARM_WORKAROUND to a value of 1
// work around this issue.
//
// The cost of this define is in correctness of managed stack traces.
// With this define enabled, managed stack traces maybe not be correct
// in some cases, because the stack trace generation code must use
// fuzzy heuristics to detemine if a given instrion pointer is in a
// managed method.
#if X_ENCRYPT_TARGET_ARM64 || X_ENCRYPT_TARGET_ARMV7
#ifndef X_ENCRYPT_LARGE_EXECUTABLE_ARM_WORKAROUND
#define X_ENCRYPT_LARGE_EXECUTABLE_ARM_WORKAROUND 0
#endif
#endif

#define X_ENCRYPT_BINARY_SECTION_NAME "xfe"
#if defined(_MSC_VER)
    #define X_ENCRYPT_TARGET_WINDOWS 1

    #if X_ENCRYPT_LARGE_EXECUTABLE_ARM_WORKAROUND
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS 0
    #else
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS !X_ENCRYPT_MONO_DEBUGGER
    #endif

    #if X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
        #define X_ENCRYPT_METHOD_ATTR  __declspec(code_seg (X_ENCRYPT_BINARY_SECTION_NAME))
    #endif
    #if defined(_XBOX_ONE)
        #define X_ENCRYPT_TARGET_XBOXONE 1
        #define X_ENCRYPT_ENABLE_PLATFORM_THREAD_AFFINTY 1
    #elif defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
        #define X_ENCRYPT_TARGET_WINRT 1
    #elif defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_GAMES)
        #define X_ENCRYPT_TARGET_WINDOWS_GAMES 1
        #define X_ENCRYPT_ENABLE_PLATFORM_THREAD_AFFINTY 1
    #elif (X_ENCRYPT_CUSTOM_PLATFORM)

    #else
        #define X_ENCRYPT_TARGET_WINDOWS_DESKTOP 1
        // Windows 7 is the min OS we support, so we cannot link newer APIs
        #define NTDDI_VERSION    0x06010000
        #define _WIN32_WINNT     0x0601
        #define WINVER           0x0601
    #endif

    #define _UNICODE 1
    #define UNICODE 1
    #define STRICT 1
#elif defined(__APPLE__)
    #define X_ENCRYPT_TARGET_DARWIN 1

    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_OS_TV || TARGET_TVOS_SIMULATOR
        #define X_ENCRYPT_TARGET_IOS 1
    #else
        #define X_ENCRYPT_TARGET_OSX 1
    #endif

    #if X_ENCRYPT_LARGE_EXECUTABLE_ARM_WORKAROUND
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS 0
    #else
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS (!(X_ENCRYPT_TARGET_IOS && X_ENCRYPT_TARGET_ARMV7) && !X_ENCRYPT_MONO_DEBUGGER)
    #endif

    #if X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
        #define X_ENCRYPT_METHOD_ATTR __attribute__((section ("__TEXT," X_ENCRYPT_BINARY_SECTION_NAME ",regular,pure_instructions")))
    #endif

// because it's android based, __ANDROID__ is *also* defined on Lumin.
// so we need to check for that *before* we check __ANDROID__ to avoid false
// positives.
#elif defined(LUMIN)
    #define X_ENCRYPT_ENABLE_PLATFORM_THREAD_RENAME 1
    #define X_ENCRYPT_TARGET_LUMIN 1
    #define X_ENCRYPT_USE_GENERIC_DEBUG_LOG 0
    #define X_ENCRYPT_SUPPORTS_PROCESS 1
#elif defined(__ANDROID__)
    #define X_ENCRYPT_TARGET_ANDROID 1
    #define X_ENCRYPT_ENABLE_PLATFORM_THREAD_RENAME 1
    #if X_ENCRYPT_LARGE_EXECUTABLE_ARM_WORKAROUND
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS 0
    #else
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS !X_ENCRYPT_MONO_DEBUGGER
    #endif

    #define X_ENCRYPT_PLATFORM_DISABLE_LIBC_PINVOKE 1
    #if X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
        #define X_ENCRYPT_METHOD_ATTR __attribute__((section(X_ENCRYPT_BINARY_SECTION_NAME)))
    #endif
#elif defined(__EMSCRIPTEN__)
    #define X_ENCRYPT_TARGET_JAVASCRIPT 1
#elif defined(__linux__)
    #define X_ENCRYPT_TARGET_LINUX 1

    #if X_ENCRYPT_LARGE_EXECUTABLE_ARM_WORKAROUND
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS 0
    #else
        #define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS !X_ENCRYPT_MONO_DEBUGGER
    #endif

    #if X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
        #define X_ENCRYPT_METHOD_ATTR __attribute__((section(X_ENCRYPT_BINARY_SECTION_NAME)))
    #endif
#elif defined(NN_PLATFORM_CTR)
    #define X_ENCRYPT_TARGET_N3DS 1
#elif X_ENCRYPT_TARGET_CUSTOM
// defined handled externally
#else
    #error please define your target platform
#endif

#if X_ENCRYPT_TARGET_PS5
#define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS 1
#define X_ENCRYPT_METHOD_ATTR __attribute__((section(X_ENCRYPT_BINARY_SECTION_NAME)))
#endif

#ifndef X_ENCRYPT_TARGET_WINDOWS
#define X_ENCRYPT_TARGET_WINDOWS 0
#endif

#ifndef X_ENCRYPT_TARGET_WINDOWS_DESKTOP
#define X_ENCRYPT_TARGET_WINDOWS_DESKTOP 0
#endif

#ifndef X_ENCRYPT_TARGET_WINDOWS_GAMES
#define X_ENCRYPT_TARGET_WINDOWS_GAMES 0
#endif

#ifndef X_ENCRYPT_TARGET_WINRT
#define X_ENCRYPT_TARGET_WINRT 0
#endif

#ifndef X_ENCRYPT_TARGET_XBOXONE
#define X_ENCRYPT_TARGET_XBOXONE 0
#endif

#ifndef X_ENCRYPT_TARGET_DARWIN
#define X_ENCRYPT_TARGET_DARWIN 0
#endif

#ifndef X_ENCRYPT_TARGET_IOS
#define X_ENCRYPT_TARGET_IOS 0
#endif

#ifndef X_ENCRYPT_TARGET_OSX
#define X_ENCRYPT_TARGET_OSX 0
#endif

#ifndef X_ENCRYPT_TARGET_ANDROID
#define X_ENCRYPT_TARGET_ANDROID 0
#endif

#ifndef X_ENCRYPT_TARGET_JAVASCRIPT
#define X_ENCRYPT_TARGET_JAVASCRIPT 0
#endif

#ifndef X_ENCRYPT_TARGET_LINUX
#define X_ENCRYPT_TARGET_LINUX 0
#endif

#ifndef X_ENCRYPT_TARGET_N3DS
#define X_ENCRYPT_TARGET_N3DS 0
#endif

#ifndef X_ENCRYPT_TARGET_PS4
#define X_ENCRYPT_TARGET_PS4 0
#endif

#ifndef X_ENCRYPT_TARGET_PSP2
#define X_ENCRYPT_TARGET_PSP2 0
#endif

#ifndef X_ENCRYPT_TARGET_SWITCH
#define X_ENCRYPT_TARGET_SWITCH 0
#endif

#ifndef X_ENCRYPT_TARGET_LUMIN
#define X_ENCRYPT_TARGET_LUMIN 0
#endif

#ifndef X_ENCRYPT_TARGET_POSIX
#define X_ENCRYPT_TARGET_POSIX (X_ENCRYPT_TARGET_DARWIN || X_ENCRYPT_TARGET_JAVASCRIPT || X_ENCRYPT_TARGET_LINUX || X_ENCRYPT_TARGET_ANDROID || X_ENCRYPT_TARGET_PS4 || X_ENCRYPT_TARGET_PSP2 || X_ENCRYPT_TARGET_LUMIN)
#endif

#define X_ENCRYPT_TINY_DEBUGGER (X_ENCRYPT_TINY && X_ENCRYPT_MONO_DEBUGGER)

#define X_ENCRYPT_X_ENCRYPT_TINY_SUPPORT_THREADS X_ENCRYPT_TINY && X_ENCRYPT_TINY_DEBUGGER
#define X_ENCRYPT_X_ENCRYPT_TINY_SUPPORT_SOCKETS X_ENCRYPT_TINY && X_ENCRYPT_TINY_DEBUGGER

#ifndef X_ENCRYPT_SUPPORT_THREADS
#define X_ENCRYPT_SUPPORT_THREADS ((!X_ENCRYPT_TARGET_JAVASCRIPT || X_ENCRYPT_TINY_DEBUGGER) && (!X_ENCRYPT_TINY || X_ENCRYPT_X_ENCRYPT_TINY_SUPPORT_THREADS))
#endif

#ifndef X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS
#define X_ENCRYPT_PLATFORM_SUPPORTS_CUSTOM_SECTIONS 0
#endif

#ifndef X_ENCRYPT_DEBUG
#define X_ENCRYPT_DEBUG 0
#endif

#define X_ENCRYPT_USE_STD_THREAD 0

#define X_ENCRYPT_THREADS_STD X_ENCRYPT_USE_STD_THREAD
#define X_ENCRYPT_THREADS_PTHREAD (!X_ENCRYPT_THREADS_STD && X_ENCRYPT_TARGET_POSIX)
#define X_ENCRYPT_THREADS_WIN32 (!X_ENCRYPT_THREADS_STD && X_ENCRYPT_TARGET_WINDOWS)
#define X_ENCRYPT_THREADS_N3DS (!X_ENCRYPT_THREADS_STD && X_ENCRYPT_TARGET_N3DS)
#define X_ENCRYPT_THREADS_PS4 (!X_ENCRYPT_THREADS_STD && X_ENCRYPT_TARGET_PS4)
#define X_ENCRYPT_THREADS_PSP2 (!X_ENCRYPT_THREADS_STD && X_ENCRYPT_TARGET_PSP2)
#define X_ENCRYPT_THREADS_SWITCH (!X_ENCRYPT_THREADS_STD && X_ENCRYPT_TARGET_SWITCH)

#define X_ENCRYPT_THREAD_HAS_CPU_SET X_ENCRYPT_TARGET_POSIX && !X_ENCRYPT_THREADS_PS4

/* Trigger assert if 'ptr' is not aligned to 'alignment'. */
#define X_ENCRYPT_ASSERT_ALIGNMENT(ptr, alignment) \
    X_ENCRYPT_ASSERT((((ptrdiff_t) ptr) & (alignment - 1)) == 0 && "Unaligned pointer!")

#if defined(_MSC_VER)
    #if defined(_M_X64) || defined(_M_ARM64)
        #define X_ENCRYPT_SIZEOF_VOID_P 8
    #elif defined(_M_IX86) || defined(_M_ARM)
        #define X_ENCRYPT_SIZEOF_VOID_P 4
    #else
        #error invalid windows architecture
    #endif
#elif defined(__GNUC__) || defined(__SNC__)
    #if defined(__x86_64__)
        #define X_ENCRYPT_SIZEOF_VOID_P 8
    #elif defined(__i386__)
        #define X_ENCRYPT_SIZEOF_VOID_P 4
    #elif defined(__EMSCRIPTEN__)
        #define X_ENCRYPT_SIZEOF_VOID_P 4
    #elif defined(__arm__)
        #define X_ENCRYPT_SIZEOF_VOID_P 4
    #elif defined(__arm64__) || defined(__aarch64__)
        #define X_ENCRYPT_SIZEOF_VOID_P 8
    #else
        #error invalid windows architecture
    #endif
#else
    #error please define your target architecture size
#endif

#ifndef X_ENCRYPT_USE_GENERIC_CRASH_HELPERS
#define X_ENCRYPT_USE_GENERIC_CRASH_HELPERS (!X_ENCRYPT_TARGET_WINDOWS && !X_ENCRYPT_TARGET_POSIX)
#endif

#ifndef X_ENCRYPT_SUPPORTS_CONSOLE_EXTENSION
#define X_ENCRYPT_SUPPORTS_CONSOLE_EXTENSION X_ENCRYPT_TARGET_ANDROID
#endif

#define X_ENCRYPT_COMPILER_MSVC (X_ENCRYPT_TARGET_WINDOWS || X_ENCRYPT_TARGET_XBOXONE)

#if X_ENCRYPT_COMPILER_MSVC
#ifndef STDCALL
#define STDCALL __stdcall
#endif
#ifndef CDECL
#define CDECL __cdecl
#endif
#ifndef FASTCALL
#define FASTCALL __fastcall
#endif
#ifndef THISCALL
#define THISCALL __thiscall
#endif
#else
#define STDCALL
#define CDECL
#define FASTCALL
#define THISCALL
#endif
