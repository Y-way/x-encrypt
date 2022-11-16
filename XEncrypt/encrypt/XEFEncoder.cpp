#include "XEFEncoder.h"
#include "XService.h"
#include "common/Logging.h"
#include "common/Allocator.h"
#include "common/ZipUtils.h"
#include "common/Common.h"

namespace encrypt
{
    void XEFEncoder::Encode(XContext* context, uint8_t encryptSize, XEncodeType type)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if (context->GetType() != XContextType::Ecrypt)
        {
            context->SetResultCode(ResultCode::ContextTypeError);
            return;
        }
#ifdef XEF_ENCRYPT_SERVICE
        byte* input = const_cast<byte*>(context->GetOriginalData());
        int64_t inSize = context->GetOriginalDataLength();
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

        if (XService::IsEncrypted(input, inSize))//XEncode file.
        {
            context->SetResultCode(ResultCode::EncryptedData);
            return;
        }

        XEFHeader xHeader;
        memset(&xHeader, 0, sizeof(XEFHeader));
        xHeader.sign = (*((unsigned*)(&kXFileSignatureCode)));
        xHeader.offset = sizeof(XEFHeader);
        xHeader.encode_type = (uint8_t)type;
        xHeader.version = 1;
        xHeader.encrypt_data_size = encryptSize;
        byte* xefData = nullptr;
        byte* encodeData = nullptr;
        unsigned long dataSize = 0;
        if (xHeader.encode_type == XEncodeType::XZip)
        {
            dataSize = ZipUtils::CompressMemoryBound((unsigned long)inSize);
            xefData = (byte*)XMEMORY_MALLOC(xHeader.offset + dataSize);
            if (xefData == nullptr)
            {
                context->SetResultCode(ResultCode::OutMemory);
                return;
            }
            encodeData = xefData + xHeader.offset;
            if (!ZipUtils::CompressMemory(input, inSize, encodeData, &dataSize))
            {
                XMEMORY_FREE(xefData);
                context->SetResultCode(ResultCode::InvalidZip);
                return;
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
        byte* encryptData = rawdata;
        //encrypt the data.
        for (int i = 0; i < size; i++)
        {
            byte c = *encryptData;
            *encryptData = ((c << 0x04) | (c >> 0x04));
            encryptData++;
        }
#endif // XEF_ENCRYPT_SERVICE
        return rawdata;
    }

}