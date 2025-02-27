#include "XService.h"
#include "common/Allocator.h"
#include "common/Logging.h"
#include "XEFDecoder.h"
#include "common/Common.h"
#include "XEFEncoder.h"

namespace xencrypt
{
    static XService* s_instance = nullptr;

    XService::~XService()
    {
        UnregisterEncoder();
        UnregisterDecoder();
    }
    
    bool XService::IsEncrypted(const byte* data, int64_t size)
    {
        if (data == nullptr || size < sizeof(XEFHeader))
        {
            return false;
        }
        XEFHeader* xHeader = (XEFHeader*)data;
        bool ret = IS_XFILE_SIGNATURE_CODE(xHeader->sign);
        Logging::Write("IsEncrypted:%s", (ret ? "True" : "False"));
        return ret;
    }

    void XService::Initialize()
    {
        if (s_instance == nullptr)
        {
            s_instance = new XService();
            s_instance->RegisterEncoder<XEFEncoder>();
            s_instance->RegisterDecoder<XEFDecoder>();
        }
    }

    void XService::UnInitialize()
    {
        if (s_instance != nullptr)
        {
            delete s_instance;
            s_instance = nullptr;
        }
    }

    void XService::UnregisterDecoder()
    {
        if(_decoder != nullptr)
        {
            delete _decoder;
            _decoder = nullptr;
        }
    }

    void XService::UnregisterEncoder()
    {
        if(_encoder != nullptr)
        {
            delete _encoder;
            _encoder = nullptr;
        }
    }

    ResultCode XService::Decrypt(XContext* context, const byte* input, int64_t length, bool cloneInput /*= false*/)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if (s_instance == nullptr)
        {
            return ResultCode::UnInitialize;
        }
        if (s_instance->_decoder == nullptr)
        {
            context->SetResultCode(ResultCode::InvalidDecoder);
        }
        else
        {
            context->SetCloneInputDataFlag(cloneInput);
            context->SetInputData(input, length);
            s_instance->_decoder->Decode(context);
        }
        return context->GetResultCode();
    }

    ResultCode XService::Encrypt(XContext* context, const byte* in, int64_t length, uint8_t encryptSize, XEncodeType type)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if (s_instance == nullptr)
        {
            return ResultCode::UnInitialize;
        }
        if (s_instance->_encoder == nullptr)
        {
            context->SetResultCode(ResultCode::InvalidEncoder);
        }
        else
        {
            context->SetInputData(in, length);
            s_instance->_encoder->Encode(context, encryptSize, type);
        }
        return context->GetResultCode();
    }

    XContext* XService::CreateContext(XContextType type)
    {
        XContext* result = new XContext(type);
        if (result == nullptr)
        {
            Logging::Write("Out memory.");
            return nullptr;
        }
        return result;
    }

    void XService::ReleaseContext(XContext* context)
    {
        if (context == nullptr)
        {
            return;
        }
        delete context;
        context = nullptr;
    }
}
