#include "common/Logging.h"
#include "common/Allocator.h"
#include "common/ZipUtils.h"
#include "service/Common.h"
#include "service/XService.h"

#include "XEFDecoder.h"
#include "XEFConsts.h"

namespace xencrypt
{
    namespace
    {
        class CopyDataScope
        {
        public:
            CopyDataScope(const byte* data, int64_t size):_holder(nullptr)
            {
                _holder = (byte*)XMEMORY_MALLOC(size);
                memcpy(_holder, data, size);
            }

            byte* GetData() const { return _holder; }

            ~CopyDataScope()
            {
                if (_holder != nullptr)
                {
                    XMEMORY_FREE(_holder);
                }
                _holder = nullptr;
            }

        private:
            CopyDataScope(const CopyDataScope& other);
            CopyDataScope& operator= (const CopyDataScope& other);

        private:
            byte* _holder;
        };
    }


    bool XEFDecoder::DecryptData(XContext* context, byte* data, byte** out, size_t& unpackedLen)
    {
#ifdef XEF_DECRYPT_SERVICE
        XEFHeader* xHeader = (XEFHeader*)data;

        byte* rawdata = (byte*)(data + xHeader->offset);
        
        //decrypt the data.
        for (int i = 0; i < xHeader->encrypt_data_size; i++)
        {
            byte* c = &rawdata[i];
            *c = ((*c << 0x04) | (*c >> 0x04));
        }

        byte* unpackedData = rawdata;
        unpackedLen = xHeader->length;
        if (xHeader->encode_type == XEncodeType::XGZip)
        {
            //Unzip the compressed data
            if (!ZipUtils::IsGZipBuffer(rawdata, xHeader->length))
            {
                context->SetResultCode(ResultCode::InvalidUnzip);
                return false;
            }
            if (!ZipUtils::GZipUncompress(rawdata, xHeader->length, &unpackedData, &unpackedLen))
            {
                context->SetResultCode(ResultCode::InvalidUnzip);
                return false;
            }
            context->SetMemoryType(XCodeMemoryType::AllocateMemory);
        }
        if (out != nullptr)
        {
            *out = unpackedData;
        }
#endif
        return true;
    }

    void XEFDecoder::Decode(XContext* context)
    {
        X_ENCRYPT_ASSERT(context != nullptr);
        if(context->GetType() != XContextType::XDecrypt)
        {
            context->SetResultCode(ResultCode::ContextTypeError);
            return;
        }

#ifdef XEF_DECRYPT_SERVICE
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

        if (XService::IsEncrypted(input, inSize))//Decrypt data.
        {
            if (context->IsCloneInputData())
            {
                //copy the original data.
                CopyDataScope scope(context->GetInputData(), inSize);
                if (!DecryptData(context, scope.GetData(), &unpackedData, unpackedLen))
                {
                    return;
                }
            }
            else
            {
                if (!DecryptData(context, input, &unpackedData, unpackedLen))
                {
                    return;
                }
            }
        }
        context->SetResultData(unpackedData, unpackedLen);
        context->SetResultCode(ResultCode::Ok);
#else
        context->SetResultCode(ResultCode::NotSupportDecrypt);
#endif // !XEF_DECRYPT_SERVICE
    }
}
