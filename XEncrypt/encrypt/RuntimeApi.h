#pragma once

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

#define EXPORT_API(r, n, p)             X_ENCRYPT_EXPORT r n p;

    EXPORT_API(void, decrypt_service_initialize, ());
    EXPORT_API(void, decrypt_service_deinitialize, ());
    EXPORT_API(void*, decrypt_create_xcontext, (int type));
    EXPORT_API(bool, decrypt_service_is_encrypted, (const byte* in, int64_t in_size));
    EXPORT_API(int, decrypt_service_encrypt, (void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size, uint8_t encryptSize, int type));
    EXPORT_API(int, decrypt_service_decrypt, (void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size));
    EXPORT_API(void, decrypt_release_xcontext, (void* context));

// #undef EXPORT_API
#if defined(__cplusplus)
}
#endif // __cplusplus



