#pragma once

#include <string.h>

/* first setup platform defines*/
#include "config_platforms.h"
#include "config_api_platforms.h"

#include "config_api.h"

#if !defined(XEF_DECRYPT_SERVICE) && !defined(XEF_ENCRYPT_SERVICE)
#error "Must define at least one of XEF_DECRYPT_SERVICE or XEF_ENCRYPT_SERVICE"
#endif

#ifdef XENCRYPT_EXPORT_API
# define XENCRYPT_API X_ENCRYPT_EXPORT
#elif XENCRYPT_IMPORT_API
# define XENCRYPT_API X_ENCRYPT_IMPORT
#else
# define XENCRYPT_API
#endif

#if defined(__ARMCC_VERSION)
    #include <wchar.h>
    #include <ctype.h>
    #define INTPTR_MAX 2147483647
#endif

#ifndef X_ENCRYPT_METHOD_ATTR
#define X_ENCRYPT_METHOD_ATTR
#endif

#if defined(_MSC_VER)
#define X_ENCRYPT_CXX_ABI_MSVC 1
#else
#define X_ENCRYPT_CXX_ABI_MSVC 0
#endif

#if defined(__cplusplus)
#define X_ENCRYPT_EXTERN_C extern "C"
#define X_ENCRYPT_EXTERN_C_CONST extern "C" const
#define X_ENCRYPT_EXTERN_C_BEGIN extern "C" {
#define X_ENCRYPT_EXTERN_C_END }
#else
#define X_ENCRYPT_EXTERN_C
#define X_ENCRYPT_EXTERN_C_CONST extern const
#define X_ENCRYPT_EXTERN_C_BEGIN
#define X_ENCRYPT_EXTERN_C_END
#endif

#if X_ENCRYPT_COMPILER_MSVC || defined(__ARMCC_VERSION)
#define X_ENCRYPT_NO_INLINE __declspec(noinline)
#define X_ENCRYPT_NO_ALIAS __declspec(noalias)
#define X_ENCRYPT_PARAMETER_RESTRICT __restrict
#define X_ENCRYPT_METHOD_RESTRICT __declspec(restrict)
#define X_ENCRYPT_ASSUME(x) __assume(x)
#else
#define X_ENCRYPT_NO_INLINE __attribute__ ((noinline))
#define X_ENCRYPT_NO_ALIAS
#define X_ENCRYPT_PARAMETER_RESTRICT
#define X_ENCRYPT_METHOD_RESTRICT
#define X_ENCRYPT_ASSUME(x)
#endif

#if X_ENCRYPT_COMPILER_MSVC
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

#define X_ENCRYPT_ENABLE_MONO_BUG_EMULATION 1

#ifndef ALIGN_OF // Baselib header can also define this - if so use their definition.
#if defined(__GNUC__) || defined(__SNC__) || defined(__clang__)
    #define ALIGN_OF(T) __alignof__(T)
    #define ALIGN_TYPE(val) __attribute__((aligned(val)))
    #define ALIGN_FIELD(val) ALIGN_TYPE(val)
    #define X_ENCRYPT_FORCE_INLINE inline __attribute__ ((always_inline))
    #define X_ENCRYPT_MANAGED_FORCE_INLINE X_ENCRYPT_FORCE_INLINE
#elif defined(_MSC_VER)
    #define ALIGN_OF(T) __alignof(T)
#if _MSC_VER >= 1900 && defined(__cplusplus)
    #define ALIGN_TYPE(val) alignas(val)
#else
    #define ALIGN_TYPE(val) __declspec(align(val))
#endif
    #define ALIGN_FIELD(val) __declspec(align(val))
    #define X_ENCRYPT_FORCE_INLINE __forceinline
    #define X_ENCRYPT_MANAGED_FORCE_INLINE inline
#else
    #define ALIGN_TYPE(size)
    #define ALIGN_FIELD(size)
    #define X_ENCRYPT_FORCE_INLINE inline
    #define X_ENCRYPT_MANAGED_FORCE_INLINE X_ENCRYPT_FORCE_INLINE
#endif
#endif

#define X_ENCRYPT_PAGE_SIZE 4096

// 64-bit types are aligned to 8 bytes on 64-bit platforms
#define X_ENCRYPT_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT (X_ENCRYPT_SIZEOF_VOID_P == 8)

/* Debugging */
#ifndef X_ENCRYPT_DEBUG
#define X_ENCRYPT_DEBUG 0
#endif

#ifndef X_ENCRYPT_DEVELOPMENT
#define X_ENCRYPT_DEVELOPMENT 0
#endif

#define X_ENCRYPT_THREADS_ALL_ACCESS (!X_ENCRYPT_THREADS_STD && X_ENCRYPT_TARGET_XBOXONE)

#if (X_ENCRYPT_SUPPORT_THREADS && (!X_ENCRYPT_THREADS_STD && !X_ENCRYPT_THREADS_PTHREAD && !X_ENCRYPT_THREADS_WIN32 && !X_ENCRYPT_THREADS_XBOXONE && !X_ENCRYPT_THREADS_N3DS && !X_ENCRYPT_THREADS_PS4 && !X_ENCRYPT_THREADS_PSP2 && !X_ENCRYPT_THREADS_SWITCH && !X_ENCRYPT_THREADS_CUSTOM))
#error "No thread implementation defined"
#endif

/* Platform support to cleanup attached threads even when native threads are not exited cleanly */
#define X_ENCRYPT_HAS_NATIVE_THREAD_CLEANUP (X_ENCRYPT_THREADS_PTHREAD || X_ENCRYPT_THREADS_WIN32 || X_ENCRYPT_TARGET_SWITCH)

#define X_ENCRYPT_THREAD_IMPL_HAS_COM_APARTMENTS X_ENCRYPT_TARGET_WINDOWS

#if !defined(X_ENCRYPT_ENABLE_PLATFORM_THREAD_STACKSIZE) && X_ENCRYPT_TARGET_IOS
#define X_ENCRYPT_ENABLE_PLATFORM_THREAD_STACKSIZE 1
#endif

#if X_ENCRYPT_TINY
    #if X_ENCRYPT_TINY_DEBUG_METADATA
        #define X_ENCRYPT_ENABLE_STACKTRACES 1
    #else
        #define X_ENCRYPT_ENABLE_STACKTRACES 0
    #endif // X_ENCRYPT_TINY_DEBUG_METADATA
#else
    #define X_ENCRYPT_ENABLE_STACKTRACES 1
#endif // X_ENCRYPT_TINY


#ifndef HOST_WASM
#if X_ENCRYPT_TINY_DEBUGGER && X_ENCRYPT_TARGET_JAVASCRIPT
#define HOST_WASM 1
#endif
#endif

/* Platforms which use OS specific implementation to extract stracktrace */
#if !defined(X_ENCRYPT_ENABLE_NATIVE_STACKTRACES)
#define X_ENCRYPT_ENABLE_NATIVE_STACKTRACES (X_ENCRYPT_TARGET_WINDOWS || X_ENCRYPT_TARGET_LINUX || X_ENCRYPT_TARGET_DARWIN || X_ENCRYPT_TARGET_IOS || X_ENCRYPT_TARGET_ANDROID || X_ENCRYPT_TARGET_LUMIN)
#endif

/* Platforms which support native IP emission to crash reporting to enable server-side reconstruction of C# exception stack trace line numbers */
#if !defined(X_ENCRYPT_ENABLE_NATIVE_INSTRUCTION_POINTER_EMISSION)
#define X_ENCRYPT_ENABLE_NATIVE_INSTRUCTION_POINTER_EMISSION (X_ENCRYPT_TARGET_WINDOWS || X_ENCRYPT_TARGET_LINUX || X_ENCRYPT_TARGET_DARWIN || X_ENCRYPT_TARGET_IOS || X_ENCRYPT_TARGET_ANDROID)
#endif

#if X_ENCRYPT_ENABLE_STACKTRACES

/* Platforms which use stacktrace sentries */
#if !defined(X_ENCRYPT_ENABLE_STACKTRACE_SENTRIES)
#define X_ENCRYPT_ENABLE_STACKTRACE_SENTRIES (X_ENCRYPT_TARGET_JAVASCRIPT || X_ENCRYPT_TARGET_N3DS || X_ENCRYPT_TARGET_SWITCH)
#endif

#endif // X_ENCRYPT_ENABLE_STACKTRACES

#if (X_ENCRYPT_ENABLE_STACKTRACES && !X_ENCRYPT_ENABLE_NATIVE_STACKTRACES && !X_ENCRYPT_ENABLE_STACKTRACE_SENTRIES)
#error "If stacktraces are supported, then either native stack traces must be supported, or usage of stacktrace sentries must be enabled!"
#endif

#if (X_ENCRYPT_ENABLE_NATIVE_STACKTRACES + X_ENCRYPT_ENABLE_STACKTRACE_SENTRIES) > 1
#error "Only one type of stacktraces are allowed"
#endif

#define X_ENCRYPT_CAN_USE_MULTIPLE_SYMBOL_MAPS X_ENCRYPT_TARGET_IOS

/* we always need to NULL pointer free memory with our current allocators */
#define NEED_TO_ZERO_PTRFREE 1
#define X_ENCRYPT_HAS_GC_DESCRIPTORS 1

#if defined(_MSC_VER)
    #define X_ENCRYPT_ZERO_LEN_ARRAY 0
#else
    #define X_ENCRYPT_ZERO_LEN_ARRAY 0
#endif

#if defined(_MSC_VER)
    #define X_ENCRYPT_HAS_CXX_CONSTEXPR (_MSC_VER >= 1900)
#else
    #define X_ENCRYPT_HAS_CXX_CONSTEXPR (__has_feature (cxx_constexpr))
#endif

#if X_ENCRYPT_HAS_CXX_CONSTEXPR
    #define COMPILE_TIME_CONST constexpr
#else
    #define COMPILE_TIME_CONST const
#endif

/* clang specific __has_builtin check */
#ifndef __has_builtin
    #define __has_builtin(x) 0 // Compatibility with non-clang compilers.
#endif

#if _MSC_VER
#define X_ENCRYPT_UNREACHABLE __assume(0)
#elif __has_builtin(__builtin_unreachable)
#define X_ENCRYPT_UNREACHABLE __builtin_unreachable()
#else
#define X_ENCRYPT_UNREACHABLE
#endif

/* Debug macros */

#if defined(_MSC_VER) && _MSC_VER >= 1900 // UTF-8 literals are only supported in VS2015+
// On MSVC, __FILE__ will expand to ANSI string literal and will fail to compile if there are unicode characters in the path
// So we specify it to be UTF-8 literal explicitly
    #define MAKE_UTF8_LITERAL_HELPER(x) u8 ## x
    #define MAKE_UTF8_LITERAL(x) MAKE_UTF8_LITERAL_HELPER(x)
#else
    #define MAKE_UTF8_LITERAL(x) x
#endif

#define __FILE_UTF8__ MAKE_UTF8_LITERAL(__FILE__)
#define STRINGIZE(L)          #L
#define MAKE_STRING(M, L)     M(L)
#define $Line                   MAKE_STRING( STRINGIZE, __LINE__ )
#define FIXME                   "FIXME: "
#define ICALLMESSAGE(name)      __FILE_UTF8__ "(" $Line ") : FIXME: Missing internal call implementation: " name
#define RUNTIMEMESSAGE(name)    __FILE_UTF8__ "(" $Line ") : FIXME: Missing runtime implementation: " name
#define NOTSUPPORTEDICALLMESSAGE(target, name, reason)  __FILE_UTF8__ "(" $Line ") : Unsupported internal call for " target ":" name " - " reason

// Keeping this for future usage if needed.
//#if defined(_MSC_VER)
//  #define PRAGMA_MESSAGE(value) __pragma(message(value))
//#else
//  #define PRAGMA_MESSAGE(value) _Pragma(STRINGIZE(value))
//#endif

#define PRAGMA_MESSAGE(value)

#if !defined(__EMSCRIPTEN__)

#define X_ENCRYPT_NOT_IMPLEMENTED_ICALL(func) \
    PRAGMA_MESSAGE(ICALLMESSAGE(#func)) \
    X_ENCRYPT_ASSERT(0 && #func)
#define X_ENCRYPT_NOT_IMPLEMENTED_ICALL_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(ICALLMESSAGE(#func))

#define X_ENCRYPT_NOT_IMPLEMENTED(func) \
    PRAGMA_MESSAGE(RUNTIMEMESSAGE(#func)) \
    X_ENCRYPT_ASSERT(0 && #func)
#define X_ENCRYPT_NOT_IMPLEMENTED_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(RUNTIMEMESSAGE(#func))

#else

#include <stdio.h>
#include <emscripten/emscripten.h>
// emscripten's assert will throw an exception in js.
// For now, we don't want that, so just printf and move on.
    #define X_ENCRYPT_NOT_IMPLEMENTED_ICALL(func) \
    PRAGMA_MESSAGE(message(ICALLMESSAGE(#func))) \
    emscripten_log(EM_LOG_NO_PATHS | EM_LOG_CONSOLE | EM_LOG_ERROR | EM_LOG_JS_STACK, "Not implemented icall: %s\n", #func);
#define X_ENCRYPT_NOT_IMPLEMENTED_ICALL_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(message(ICALLMESSAGE(#func)))

#define X_ENCRYPT_NOT_IMPLEMENTED(func) \
    PRAGMA_MESSAGE(message(RUNTIMEMESSAGE(#func))) \
    printf("Not implemented: %s\n", #func);
#define X_ENCRYPT_NOT_IMPLEMENTED_NO_ASSERT(func, reason) \
    PRAGMA_MESSAGE(message(RUNTIMEMESSAGE(#func)))

#endif

#if X_ENCRYPT_COMPILER_MSVC
    #define X_ENCRYPT_DIR_SEPARATOR '\\'   /* backslash */
#else
    #define X_ENCRYPT_DIR_SEPARATOR '/'    /* forward slash */
#endif

#ifndef X_ENCRYPT_DISABLE_FULL_MESSAGES
    #define X_ENCRYPT_DISABLE_FULL_MESSAGES    1
#endif

#if X_ENCRYPT_COMPILER_MSVC
    #define X_ENCRYPT_USE_GENERIC_SOCKET_IMPL  0
#else
    #define X_ENCRYPT_USE_GENERIC_SOCKET_IMPL  !(X_ENCRYPT_TARGET_POSIX || X_ENCRYPT_TARGET_SWITCH || X_ENCRYPT_SUPPORT_SOCKETS_POSIX_API)
#endif

#ifndef X_ENCRYPT_USE_GENERIC_SOCKET_BRIDGE
#define X_ENCRYPT_USE_GENERIC_SOCKET_BRIDGE !X_ENCRYPT_TARGET_JAVASCRIPT
#endif

/* set by platforms that require special handling of SIGPIPE signalling during socket sends */
#ifndef X_ENCRYPT_USE_SEND_NOSIGNAL
    #define X_ENCRYPT_USE_SEND_NOSIGNAL 0
#endif

#ifndef X_ENCRYPT_USE_GENERIC_ENVIRONMENT
#define X_ENCRYPT_USE_GENERIC_ENVIRONMENT  (!X_ENCRYPT_TARGET_WINDOWS && !X_ENCRYPT_TARGET_POSIX)
#endif

#define X_ENCRYPT_USE_GENERIC_COM  (!X_ENCRYPT_TARGET_WINDOWS || X_ENCRYPT_TARGET_WINDOWS_GAMES)
#define X_ENCRYPT_USE_GENERIC_COM_SAFEARRAYS   (!X_ENCRYPT_TARGET_WINDOWS || X_ENCRYPT_TARGET_XBOXONE || X_ENCRYPT_TARGET_WINDOWS_GAMES)
#define X_ENCRYPT_USE_GENERIC_WINDOWSRUNTIME (!X_ENCRYPT_TARGET_WINDOWS || RUNTIME_NONE || X_ENCRYPT_TINY || X_ENCRYPT_TARGET_WINDOWS_GAMES)

#ifndef X_ENCRYPT_USE_GENERIC_MEMORY_MAPPED_FILE
#define X_ENCRYPT_USE_GENERIC_MEMORY_MAPPED_FILE (X_ENCRYPT_TARGET_XBOXONE || X_ENCRYPT_TARGET_WINDOWS_GAMES || X_ENCRYPT_TARGET_JAVASCRIPT || (!X_ENCRYPT_TARGET_WINDOWS && !X_ENCRYPT_TARGET_POSIX))
#endif

#ifndef X_ENCRYPT_HAS_CLOSE_EXEC
#define X_ENCRYPT_HAS_CLOSE_EXEC (X_ENCRYPT_TARGET_POSIX && !X_ENCRYPT_TARGET_PS4)
#endif

#ifndef X_ENCRYPT_HAS_DUP
#define X_ENCRYPT_HAS_DUP (X_ENCRYPT_TARGET_POSIX && !X_ENCRYPT_TARGET_PS4)
#endif

#ifndef X_ENCRYPT_USE_GENERIC_FILE
#define X_ENCRYPT_USE_GENERIC_FILE (!X_ENCRYPT_TARGET_WINDOWS && !X_ENCRYPT_TARGET_DARWIN)
#endif

#ifndef X_ENCRYPT_USE_GENERIC_DEBUG_LOG
#define X_ENCRYPT_USE_GENERIC_DEBUG_LOG !X_ENCRYPT_TARGET_WINDOWS
#endif

#ifndef X_ENCRYPT_USE_GENERIC_PROCESS
#define X_ENCRYPT_USE_GENERIC_PROCESS !X_ENCRYPT_TARGET_LUMIN
#endif

#define X_ENCRYPT_SIZEOF_STRUCT_WITH_NO_INSTANCE_FIELDS 1
#define X_ENCRYPT_VALIDATE_FIELD_LAYOUT 0

#ifndef X_ENCRYPT_USE_POSIX_COND_TIMEDWAIT_REL
#define X_ENCRYPT_USE_POSIX_COND_TIMEDWAIT_REL ( X_ENCRYPT_TARGET_DARWIN || X_ENCRYPT_TARGET_PSP2 || ( X_ENCRYPT_TARGET_ANDROID && !X_ENCRYPT_TARGET_ARM64 && !__x86_64__ ) )
#endif

#ifdef __cplusplus
template<bool value>
struct XEncryptStaticAssertHelper;

template<>
struct XEncryptStaticAssertHelper<true>
{
};


#define Assert(x) do { (void)(x); X_ENCRYPT_ASSERT(x); } while (false)
#define XEncryptStaticAssert(...) do { XEncryptStaticAssertHelper<(__VA_ARGS__)>(); } while (false)
#endif

static const int32_t kInt32Min = INT32_MIN;
static const int32_t kInt32Max = INT32_MAX;
static const uint32_t kUInt32Max = UINT32_MAX;
static const int64_t kInt64Min = INT64_MIN;
static const int64_t kInt64Max = INT64_MAX;
static const uint64_t kUInt64Max = UINT64_MAX;

#if X_ENCRYPT_SIZEOF_VOID_P == 8
static const intptr_t kIntPtrMin = INT64_MIN;
static const intptr_t kIntPtrMax = INT64_MAX;
static const uintptr_t kUIntPtrMax = UINT64_MAX;
#else
static const intptr_t kIntPtrMin = INT32_MIN;
static const intptr_t kIntPtrMax = INT32_MAX;
static const uintptr_t kUIntPtrMax = UINT32_MAX;
#endif

// Android: "There is no support for locales in the C library" https://code.google.com/p/android/issues/detail?id=57313
// PS4/PS2: strtol_d doesn't exist
#if !defined(X_ENCRYPT_SUPPORT_LOCALE_INDEPENDENT_PARSING)
#define X_ENCRYPT_SUPPORT_LOCALE_INDEPENDENT_PARSING (!X_ENCRYPT_TARGET_ANDROID && !X_ENCRYPT_TARGET_PS4 && !X_ENCRYPT_TARGET_PSP2 && !X_ENCRYPT_TARGET_LUMIN)
#endif

#ifndef NO_UNUSED_WARNING
#define NO_UNUSED_WARNING(expr) (void)(expr)
#endif // !NO_UNUSED_WARNING

#define X_ENCRYPT_LITTLE_ENDIAN 1
#define X_ENCRYPT_BIG_ENDIAN 2
#define X_ENCRYPT_BYTE_ORDER X_ENCRYPT_LITTLE_ENDIAN

#if (defined(_MSC_VER) && _MSC_VER > 1600) || (__has_feature(cxx_override_control))
#define X_ENCRYPT_OVERRIDE override
#define X_ENCRYPT_FINAL final
#else
#define X_ENCRYPT_OVERRIDE
#define X_ENCRYPT_FINAL
#endif

#if (__has_feature(cxx_deleted_functions) || (defined(_MSC_VER) && _MSC_VER >= 1800))
#define X_ENCRYPT_HAS_DELETED_FUNCTIONS 1
#else
#define X_ENCRYPT_HAS_DELETED_FUNCTIONS 0
#endif

static const byte kXFileSignatureCode[] = { '@', 'X', 'F', 'E' };
template<typename T = uint32_t>
bool IS_XFILE_SIGNATURE_CODE(T code) { return code == *((uint32_t*)(&kXFileSignatureCode)); }

#if X_ENCRYPT_COMPILER_MSVC
#define X_ENCRYPT_ATTRIBUTE_WEAK
#else
#define X_ENCRYPT_ATTRIBUTE_WEAK __attribute__((weak))
#endif

#if X_ENCRYPT_TARGET_XBOXONE || X_ENCRYPT_TARGET_WINRT || X_ENCRYPT_TARGET_ANDROID || X_ENCRYPT_TARGET_PS4 || X_ENCRYPT_TARGET_PSP2
#define X_ENCRYPT_USE_GENERIC_CPU_INFO 1
#else
#define X_ENCRYPT_USE_GENERIC_CPU_INFO 0
#endif

#if !defined(X_ENCRYPT_CAN_CHECK_EXECUTABLE)
#define X_ENCRYPT_CAN_CHECK_EXECUTABLE X_ENCRYPT_TARGET_WINDOWS || (X_ENCRYPT_TARGET_POSIX && !X_ENCRYPT_TARGET_PS4)
#endif

#ifdef _MSC_VER
#define X_ENCRYPT_DEBUG_BREAK() __debugbreak()
#else
#define X_ENCRYPT_DEBUG_BREAK()
#endif

#if defined(__cplusplus)

template<typename Type, size_t Size>
char(*xencrypt_array_size_helper(Type(&array)[Size]))[Size];

#define X_ENCRYPT_ARRAY_SIZE(array) (sizeof(*xencrypt_array_size_helper(array)))

#endif // __cplusplus

#ifndef X_ENCRYPT_MUTATE_METHOD_POINTERS
#define X_ENCRYPT_MUTATE_METHOD_POINTERS !X_ENCRYPT_TARGET_PS4
#endif

#define X_ENCRYPT_USE_GENERIC_ASSERT !(X_ENCRYPT_TARGET_WINDOWS || X_ENCRYPT_TARGET_XBOXONE || X_ENCRYPT_TARGET_WINRT || X_ENCRYPT_TARGET_PS4 || X_ENCRYPT_TARGET_PS5)

#ifndef X_ENCRYPT_USE_SPARSEHASH
#define X_ENCRYPT_USE_SPARSEHASH (X_ENCRYPT_TARGET_ANDROID || X_ENCRYPT_TARGET_IOS)
#endif

#if !X_ENCRYPT_DEBUG
#define X_ENCRYPT_ASSERT(expr) void(0)
#else
#if defined(__cplusplus)
#define X_ENCRYPT_ASSERT(expr) (static_cast<bool>(expr) ? void(0) : xencrypt_assert(#expr, __FILE__, __LINE__))
#else
#define X_ENCRYPT_ASSERT(expr) (expr) ? void(0) : xencrypt_assert(#expr, __FILE__, __LINE__))
#endif
extern void xencrypt_assert(const char* assertion, const char* file, unsigned int line);
#endif
