#include "XAssert.h"

#if X_ENCRYPT_DEBUG
#include <cstdio>
#include <cstdlib>

void xencrypt_assert(const char* assertion, const char* file, unsigned int line)
{
#if X_ENCRYPT_USE_GENERIC_ASSERT
    printf("Assertion failed: %s, file %s, line %u\n", assertion, file, line);
    abort();
#else
    X_ENCRYPT_DEBUG_BREAK
#endif // X_ENCRYPT_USE_GENERIC_ASSERT
}

#endif // X_ENCRYPT_DEBUG
