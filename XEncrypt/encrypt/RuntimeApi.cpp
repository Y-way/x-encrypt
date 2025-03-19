#include "RuntimeApi.h"
#include "service/XService.h"
#include "service/XContext.h"
#include "service/XResult.h"

using namespace xencrypt;

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

XENCRYPT_API void* xencrypt_service_initialize(void* plugin)
{
    return (void*)(new XService(reinterpret_cast<XPlugin*>(plugin)));
}

XENCRYPT_API bool xencrypt_service_is_encrypted(void* service, const byte* data, int64_t size)
{
    if (service == nullptr)
    {
        return false;
    }

    return reinterpret_cast<XService*>(service)->IsEncrypted(data, size);
}

XENCRYPT_API void xencrypt_service_deinitialize(void* service)
{
    if (service == nullptr)
    {
        return;
    }
    delete reinterpret_cast<XService*>(service);
}

XENCRYPT_API xencrypt_result xencrypt_service_encrypt(void* service, const byte* in, int64_t in_size)
{
    XService* x = reinterpret_cast<XService*>(service);
    xencrypt_result retVal = { ResultCode::Unknown, 0, nullptr, nullptr};
    if (x == nullptr)
    {
        retVal.code = ResultCode::UnInitialize;
        return retVal;
    }
    XResult* result = x->Encrypt(in, in_size);
    
    if (result != nullptr)
    {
        retVal.code = result->GetCode();
        retVal.data = result->GetData();
        retVal.size = result->GetDataSize();
    }
    retVal.result = result;
    return retVal;
}

XENCRYPT_API xencrypt_result xencrypt_service_decrypt(void* service, const byte* in, int64_t in_size)
{
    XService* x = reinterpret_cast<XService*>(service);
    xencrypt_result retVal = { ResultCode::Unknown, 0, nullptr, nullptr};
    if (x == nullptr)
    {
        retVal.code = ResultCode::UnInitialize;
        return retVal;
    }
    XResult* result = x->Decrypt(in, in_size);
    
    if (result != nullptr)
    {
        retVal.code = result->GetCode();
        retVal.data = result->GetData();
        retVal.size = result->GetDataSize();
    }
    retVal.result = result;
    return retVal;
}

XENCRYPT_API void xencrypt_service_release_result(void* service, xencrypt_result* result)
{
    if (service == nullptr || result == nullptr || result->result == nullptr)
    {
        return;
    }
    reinterpret_cast<XService*>(service)->ReleaseResult(reinterpret_cast<XResult*>(result->result));
    result->result = nullptr;
}

#if defined(__cplusplus)
}
#endif // __cplusplus
