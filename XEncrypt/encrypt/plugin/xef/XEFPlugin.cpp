#include "XEFPlugin.h"
#include "XEFHeader.h"
#include "common/Logging.h"
#include "XEFEncoder.h"
#include "XEFDecoder.h"
namespace xencrypt
{
    XEFPlugin::XEFPlugin(XEncodeType type, uint8_t encryptSize)
    {
#if XEF_ENCRYPT_SERVICE
        _encoder = new XEFEncoder(type, encryptSize);
#endif // XEF_ENCRYPT_SERVICE

#if XEF_DECRYPT_SERVICE
        _decoder = new XEFDecoder();
#endif // XEF_DECRYPT_SERVICE
    }

    XEFPlugin::~XEFPlugin()
    {
#if XEF_ENCRYPT_SERVICE
        delete _encoder;
        _encoder = nullptr;
#endif // XEF_ENCRYPT_SERVICE

#if XEF_DECRYPT_SERVICE
        delete _decoder;
        _decoder = nullptr;
#endif // XEF_DECRYPT_SERVICE
    }
    
    bool XEFPlugin::IsEncrypted(const byte* data, int64_t size)
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

    void XEFPlugin::Encrypt(XContext* context)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if (_encoder == nullptr)
        {
            context->SetResultCode(ResultCode::InvalidEncoder);
            return;
        }
        _encoder->Encode(context);
    }

    void XEFPlugin::Decrypt(XContext* context)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if (_decoder == nullptr)
        {
            context->SetResultCode(ResultCode::InvalidDecoder);
            return;
        }
        _decoder->Decode(context);
    }
}
