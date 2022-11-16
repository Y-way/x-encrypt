#pragma once

#include <stdint.h>
#include <stddef.h>

#if !defined(__cplusplus)
#define bool uint8_t
#endif // !__cplusplus

#if !__SNC__ // SNC doesn't like the following define: "warning 1576: predefined meaning of __has_feature discarded"
#ifndef __has_feature // clang specific __has_feature check
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif
#endif

#if _MSC_VER
typedef wchar_t XEncryptChar;
#elif __has_feature(cxx_unicode_literals)
typedef char16_t XEncryptChar;
#else
typedef uint16_t XEncryptChar;
#endif

#if _MSC_VER
typedef wchar_t Il2CppNativeChar;
#define X_ENCRYPT_NATIVE_STRING(str) L##str
#else
typedef char Il2CppNativeChar;
#define X_ENCRYPT_NATIVE_STRING(str) str
#endif

typedef uint8_t byte;
