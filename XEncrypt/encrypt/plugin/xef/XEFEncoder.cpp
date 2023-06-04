#include "common/Logging.h"
#include "common/Allocator.h"
#include "common/ZipUtils.h"

#include "service/Common.h"
#include "service/XService.h"


#include "XEFEncoder.h"
#include "XEFHeader.h"
#include "XEncodeType.h"
#include "XEFEncryptPlugin.h"

namespace xencrypt
{
    void XEFEncoder::Encode(XContext* context)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if (context->GetType() != XContextType::XEncrypt)
        {
            context->SetResultCode(ResultCode::ContextTypeError);
            return;
        }
#ifdef XEF_ENCRYPT_SERVICE
        byte* input = const_cast<byte*>(context->GetInputData());
        int64_t inSize = context->GetInputDataLength();
        if (input == nullptr)
        {
            context->SetResultCode(ResultCode::InvalidInputData);
            return;
        }
        if (inSize <= 0)
        {
            context->SetResultCode(ResultCode::InvalidInputDataSize);
            return;
        }

        byte* unpackedData = input;
        size_t unpackedLen = inSize;
        context->SetMemoryType(XCodeMemoryType::OriginalOffset);

        if (XEFEncryptPlugin::IsEncrypted(input, inSize))//XEncode file.
        {
            context->SetResultCode(ResultCode::EncryptedData);
            return;
        }

        XEFHeader xHeader;
        memset(&xHeader, 0, sizeof(XEFHeader));
        xHeader.sign = (*((unsigned*)(&kXFileSignatureCode)));
        xHeader.offset = sizeof(XEFHeader);
        xHeader.encode_type = (uint8_t)_encodeType;
        xHeader.version = 1;
        xHeader.encrypt_data_size = _encryptSize;
        byte* xefData = nullptr;
        byte* encodeData = nullptr;
        unsigned long dataSize = 0;
        if (xHeader.encode_type == XEncodeType::XGZip)
        {
            dataSize = (unsigned long)inSize;
            //data too small.
            if (dataSize < 32)
            {
                dataSize = 32;
            }
            dataSize = ZipUtils::CompressMemoryBound(dataSize);
            xefData = (byte*)XMEMORY_MALLOC(xHeader.offset + dataSize);
            if (xefData == nullptr)
            {
                context->SetResultCode(ResultCode::OutMemory);
                return;
            }

            bool tryAgain = false;
            encodeData = xefData + xHeader.offset;
            if (!ZipUtils::GZipCompress(input, inSize, encodeData, &dataSize))
            {
                XMEMORY_FREE(xefData);
                context->SetResultCode(ResultCode::InvalidZip);

                tryAgain = true;
            }

            if (tryAgain)
            {
                std::string outData;
                int retCode = ZipUtils::GZipCompress(input, inSize, outData);
                if (retCode != 0)
                {
                    context->SetResultCode(ResultCode::InvalidZip);
                    return;
                }
                dataSize = (unsigned long)outData.length();
                xefData = (byte*)XMEMORY_MALLOC(xHeader.offset + dataSize);
                if (xefData == nullptr)
                {
                    context->SetResultCode(ResultCode::OutMemory);
                    return;
                }
                encodeData = xefData + xHeader.offset;
                memcpy(encodeData, outData.data(), dataSize);
            }

            context->SetMemoryType(XCodeMemoryType::AllocateMemory);
        }
        else
        {
            xefData = (byte*)XMEMORY_MALLOC(xHeader.offset + inSize);
            if (xefData == nullptr)
            {
                context->SetResultCode(ResultCode::OutMemory);
                return;
            }
            dataSize = (unsigned long)inSize;
            encodeData = xefData + xHeader.offset;
            memcpy(encodeData, input, inSize);
            context->SetMemoryType(XCodeMemoryType::AllocateMemory);
        }

        if (xHeader.encrypt_data_size > dataSize)
        {
            xHeader.encrypt_data_size = (uint8_t)dataSize;
        }
        xHeader.length = (uint32_t)dataSize;

        EncryptData(encodeData, xHeader.encrypt_data_size);
        memcpy(xefData, &xHeader, sizeof(XEFHeader));
        context->SetResultData(xefData, xHeader.offset + xHeader.length);
        context->SetResultCode(ResultCode::Ok);
#else
        context->SetResultCode(ResultCode::NotSupportEncrypt);
#endif // !XEF_ENCRYPT_SERVICE
    }
    
    byte* XEFEncoder::EncryptData(byte* rawdata, int size)
    {
#ifdef XEF_ENCRYPT_SERVICE
        X_ENCRYPT_ASSERT(rawdata != NULL);
        //encrypt the data.
        for (int i = 0; i < size; i++)
        {
            byte* c = &rawdata[i];
            *c = ((*c << 0x04) | (*c >> 0x04));
        }
#endif // XEF_ENCRYPT_SERVICE
        return rawdata;
    }

}