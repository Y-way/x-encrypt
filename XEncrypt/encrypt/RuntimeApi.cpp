#include "RuntimeApi.h"
#include "service/XService.h"
#include "service/XContext.h"
#include "plugin/xef/XEFDecoder.h"

using namespace encrypt;

void decrypt_service_initialize()
{
    XService::Initialize();
}

void decrypt_service_deinitialize()
{
    XService::UnInitialize();
}

void* decrypt_create_xcontext(int type)
{
    return (void*)XService::CreateContext((XContextType)type);
}

void decrypt_release_xcontext(void* context)
{
    XService::ReleaseContext(reinterpret_cast<XContext*>(context));
}

bool decrypt_service_is_encrypted(const byte* in, int64_t in_size)
{
    return XService::IsEncrypted(in, in_size);
}

int decrypt_service_decrypt(void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size)
{
    XContext* x = reinterpret_cast<XContext*>(context);
    ResultCode code = XService::Decrypt(x, in, in_size);   
    int64_t dataLength = 0;
    void* pOut = nullptr;
    if (x != nullptr)
    {
        pOut = x->GetResultData();
        dataLength = x->GetResultDataLength();
    }
    if (out_size != nullptr)
    {
        *out_size = dataLength;
    }
    if (out != nullptr)
    {
        *out = pOut;
    }
    return code;
}

int decrypt_service_encrypt(void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size, uint8_t encryptSize, int type)
{
    XContext* x = reinterpret_cast<XContext*>(context);
    ResultCode code = XService::Encrypt(x, in, in_size, encryptSize, (XEncodeType)type);   
    int64_t dataLength = 0;
    void* pOut = nullptr;
    if (x != nullptr)
    {
        pOut = x->GetResultData();
        dataLength = x->GetResultDataLength();
    }
    if (out_size != nullptr)
    {
        *out_size = dataLength;
    }
    if (out != nullptr)
    {
        *out = pOut;
    }
    return code;
}
