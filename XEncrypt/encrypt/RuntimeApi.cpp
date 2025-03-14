#include "RuntimeApi.h"
#include "service/XService.h"
#include "service/XContext.h"
#include "service/XResult.h"

using namespace xencrypt;

void* xencrypt_service_initialize(void* plugin)
{
    return (void*)(new XService(reinterpret_cast<XPlugin*>(plugin)));
}

bool xencrypt_service_is_encrypted(void* service, const byte* data, int64_t size)
{
    if (service == nullptr)
    {
        return false;
    }

    return reinterpret_cast<XService*>(service)->IsEncrypted(data, size);
}

void xencrypt_service_deinitialize(void* service)
{
    if (service == nullptr)
    {
        return;
    }
    delete reinterpret_cast<XService*>(service);
}

void* xencrypt_service_decrypt(void* service, const byte* in, int64_t in_size, int* code, void** out, int64_t* out_size)
{
    XService* x = reinterpret_cast<XService*>(service);
    if (x == nullptr)
    {
        if (code != nullptr)
        {
            *code = ResultCode::UnInitialize;
        }
        return nullptr;
    }
    XResult* result = x->Decrypt(in, in_size);
    int64_t dataLength = 0;
    void* pOut = nullptr;

    if (code != nullptr)
    {
        *code = ResultCode::Unknown;
    }
    if (result != nullptr)
    {
        if (code != nullptr)
        {
            *code = result->GetCode();
        }
        pOut = result->GetData();
        dataLength = result->GetDataSize();
    }
    if (out_size != nullptr)
    {
        *out_size = dataLength;
    }
    if (out != nullptr)
    {
        *out = pOut;
    }
    return result;
}

void* xencrypt_service_encrypt(void* service, const byte* in, int64_t in_size, int* code, void** out, int64_t* out_size)
{
    XService* x = reinterpret_cast<XService*>(service);
    if (x == nullptr)
    {
        if (code != nullptr)
        {
            *code = ResultCode::UnInitialize;
        }
        return nullptr;
    }
    XResult* result = x->Encrypt(in, in_size);   
    int64_t dataLength = 0;
    void* pOut = nullptr;
    if (code != nullptr)
    {
        *code = ResultCode::Unknown;
    }
    if (result != nullptr)
    {
        if (code != nullptr)
        {
            *code = result->GetCode();
        }
        pOut = result->GetData();
        dataLength = result->GetDataSize();
    }
    if (out_size != nullptr)
    {
        *out_size = dataLength;
    }
    if (out != nullptr)
    {
        *out = pOut;
    }
    return result;
}

void xencrypt_service_release_result(void* service, void* result)
{
    reinterpret_cast<XService*>(service)->ReleaseResult(reinterpret_cast<XResult*>(result));
}
