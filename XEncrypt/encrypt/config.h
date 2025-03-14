#pragma once

#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* first setup platform defines*/
#include "platforms.h"

#include "config_api.h"

#if !defined(XEF_DECRYPT_SERVICE) && !defined(XEF_ENCRYPT_SERVICE)
#error "Must define at least one of XEF_DECRYPT_SERVICE or XEF_ENCRYPT_SERVICE"
#endif

#if !defined(__cplusplus)
#define bool uint8_t
#endif // !__cplusplus

#if !__SNC__ // SNC doesn't like the following define: "warning 1576: predefined meaning of __has_feature discarded"
#ifndef __has_feature // clang specific __has_feature check
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif
#endif

typedef uint8_t byte;

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

#if defined(__ARMCC_VERSION)
    #include <wchar.h>
    #include <ctype.h>
    #define INTPTR_MAX 2147483647
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

/* Debugging */
#ifndef X_ENCRYPT_DEBUG
#define X_ENCRYPT_DEBUG 0
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

#if X_ENCRYPT_COMPILER_MSVC
    #define X_ENCRYPT_DIR_SEPARATOR '\\'   /* backslash */
#else
    #define X_ENCRYPT_DIR_SEPARATOR '/'    /* forward slash */
#endif

#ifdef __cplusplus
template<bool value>
struct XEncryptStaticAssertHelper;

template<>
struct XEncryptStaticAssertHelper<true>
{
};


#define XEncryptAssert(x) do { (void)(x); X_ENCRYPT_ASSERT(x); } while (false)
#define XEncryptStaticAssert(...) do { XEncryptStaticAssertHelper<(__VA_ARGS__)>(); } while (false)
#endif

#ifndef NO_UNUSED_WARNING
#define NO_UNUSED_WARNING(expr) (void)(expr)
#endif // !NO_UNUSED_WARNING

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

#if X_ENCRYPT_COMPILER_MSVC
#define X_ENCRYPT_ATTRIBUTE_WEAK
#else
#define X_ENCRYPT_ATTRIBUTE_WEAK __attribute__((weak))
#endif

#ifdef _MSC_VER
#define X_ENCRYPT_DEBUG_BREAK() __debugbreak()
#else
#define X_ENCRYPT_DEBUG_BREAK()
#endif

#define X_ENCRYPT_USE_GENERIC_ASSERT !(X_ENCRYPT_TARGET_WINDOWS || X_ENCRYPT_TARGET_XBOXONE || X_ENCRYPT_TARGET_WINRT || X_ENCRYPT_TARGET_PS4 || X_ENCRYPT_TARGET_PS5)

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
