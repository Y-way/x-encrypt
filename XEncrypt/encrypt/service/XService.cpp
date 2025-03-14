#include "Common.h"
#include "common/Allocator.h"
#include "XService.h"
#include "XContext.h"

namespace xencrypt
{
    bool XService::IsEncrypted(const byte* data, int64_t size)
    {
        X_ENCRYPT_ASSERT(_plugin != nullptr);
        return _plugin->IsEncrypted(data, size);
    }

    XResult* XService::Decrypt(const byte* input, int64_t length, bool cloneInput /*= false*/)
    {
        XResult* pResult = new XResult();
        X_ENCRYPT_ASSERT(pResult != nullptr);
        
        if (_plugin == nullptr)
        {
            pResult->SetResultCode(ResultCode::InvalidPlugin);
            return pResult;
        }
        
        XContext* context = new XContext(XContextType::XDecrypt, this);
        X_ENCRYPT_ASSERT(context != nullptr);
        pResult->SetContext(context);

        if (!_plugin->IsSupport(context->GetType()))
        {
            pResult->SetResultCode(ResultCode::InvalidDecoder);
            context->SetResultCode(ResultCode::InvalidDecoder);
        }
        else
        {
            context->SetCloneInputDataFlag(cloneInput);
            context->SetInputData(input, length);
            _plugin->Decrypt(context);
        }
        
        pResult->SetResultCode(context->GetResultCode());

        return pResult;
    }

    XResult* XService::Encrypt(const byte* in, int64_t length)
    {
        XResult* pResult = new XResult();
        X_ENCRYPT_ASSERT(pResult != nullptr);
        if (_plugin == nullptr)
        {
            pResult->SetResultCode(ResultCode::InvalidPlugin);
            return pResult;
        }

        XContext* context = new XContext(XContextType::XEncrypt, this);
        X_ENCRYPT_ASSERT(context != nullptr);
        pResult->SetContext(context);

        if (!_plugin->IsSupport(XContextType::XEncrypt))
        {
            pResult->SetResultCode(ResultCode::InvalidEncoder);
            context->SetResultCode(ResultCode::InvalidEncoder);
        }
        else
        {
            context->SetInputData(in, length);
            _plugin->Encrypt(context);
        }
        pResult->SetResultCode(context->GetResultCode());
        return pResult;
    }
}
