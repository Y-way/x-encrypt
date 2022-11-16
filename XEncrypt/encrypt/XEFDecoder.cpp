#include "XEFDecoder.h"
#include "XService.h"
#include "common/Logging.h"
#include "common/Allocator.h"
#include "common/ZipUtils.h"
#include "common/Common.h"

namespace encrypt
{
    void XEFDecoder::Decode(XContext* context)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if(context->GetType() != XContextType::Decrypt)
        {
            context->SetResultCode(ResultCode::ContextTypeError);
            return;
        }
#ifdef XEF_DECRYPT_SERVICE
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
            XEFHeader* xHeader = (XEFHeader*)input;
            byte* rawdata = DecryptData((byte*)(input + xHeader->offset), xHeader->encrypt_data_size);
            unpackedData = rawdata;
            unpackedLen = xHeader->length;
            if (xHeader->encode_type == XEncodeType::XZip)
            {
                //Unzip the compressed data
                if (!ZipUtils::IsGZipBuffer(rawdata, xHeader->length))
                {
                    context->SetResultCode(ResultCode::InvalidUnzip);
                    return;
                }
                if (!ZipUtils::UncompressMemory(rawdata, xHeader->length, &unpackedData, &unpackedLen))
                {
                    context->SetResultCode(ResultCode::InvalidUnzip);
                    return;
                }
                context->SetMemoryType(XCodeMemoryType::AllocateMemory);
            }
        }
        context->SetResultData(unpackedData, unpackedLen);
        context->SetResultCode(ResultCode::Ok);
#else
        context->SetResultCode(ResultCode::NotSupportDecrypt);
#endif // !XEF_DECRYPT_SERVICE
    }

    byte* XEFDecoder::DecryptData(byte* rawdata, int size)
    {
#ifdef XEF_DECRYPT_SERVICE
        X_ENCRYPT_ASSERT(rawdata != NULL);
        byte* encryptData = rawdata;
        //decrypt the data.
        for (int i = 0; i < size; i++)
        {
            byte c = *encryptData;
            *encryptData = ((c << 0x04) | (c >> 0x04));
            encryptData++;
        }
#endif // XEF_DECRYPT_SERVICE
        return rawdata;
    }
}
