// CXEncrypt.cpp: 定义应用程序的入口点。
//

#include "CXEncrypt.h"
#include "encrypt/service/XService.h"
#include "encrypt/service/XContext.h"
#include "encrypt/service/Common.h"
#include "encrypt/RuntimeApi.h"
#include "encrypt/common/Allocator.h"

#include "encrypt/RuntimeApi.h"

using namespace std;
using namespace xencrypt;

const byte rawData[] = { 'T','h','i','s',' ','a',' ','t','e','s','t',' ','d', 'a', 't', 'a', '\0', };
int64_t length = sizeof(rawData);

int main()
{
    {
        //C++
        
        //start service
        XService::Initialize();


        //Encrypting data
        // if(XService::IsEncrypted(data, length))
        // {
        //     cout>>"data has been encrypted."<<endl;
        // }

        XContext* pContext = XService::CreateContext(XContextType::XEncrypt);

        ResultCode result = XService::Encrypt(pContext, rawData, length, 32, XEncodeType::XGZip);

        const byte* encryptedData = nullptr;
        int64_t encryptedDataLength = pContext->GetResultDataLength();

        if (result != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for encryptedData.
            encryptedData = (const byte*)XMEMORY_MALLOC(encryptedDataLength);
            memcpy((void*)encryptedData, pContext->GetResultData(), encryptedDataLength);
        }

        XService::ReleaseContext(pContext);


        //Decrypting data

        // if(!XService::IsEncrypted(data, length))
        // {
        //     cout>>"data dose not have been encrypted."<<endl;
        // }


        pContext = XService::CreateContext(XContextType::XDecrypt);
        result = XService::Decrypt(pContext, encryptedData, encryptedDataLength, true);

        const byte* decryptedData = pContext->GetResultData();
        int64_t decryptedDataLength = pContext->GetResultDataLength();

        if (result != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for decryptedData.
        }
        XService::ReleaseContext(pContext);

        XMEMORY_SAFE_FREE(encryptedData);

        //stop service
        XService::UnInitialize();
    }

    {
        //C API

        //start service
        xencrypt_service_initialize();

        //Encrypting data
        // 
        // if(xencrypt_service_is_encrypted(data, length))
        // {
        //     printf("data has been encrypted.");
        // }

        void* pContext = xencrypt_create_xcontext(XContextType::XEncrypt);

        void* pEncryptBuff = nullptr;
        int64_t encryptedDataLength = 0;

        int result = xencrypt_service_encrypt(pContext, rawData, length, &pEncryptBuff, &encryptedDataLength, 32, 1);

        const byte* encryptedData = nullptr;
        if (result != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for encryptedData.
            encryptedData = (const byte*)XMEMORY_MALLOC(encryptedDataLength);
            memcpy((void*)encryptedData, pEncryptBuff, encryptedDataLength);
        }

        xencrypt_release_xcontext(pContext);


        //Decrypting data

        // if(!xencrypt_service_is_encrypted(data, length))
        // {
        //     printf("data dose not have been encrypted.");
        // }


        pContext = xencrypt_create_xcontext(XContextType::XDecrypt);

        void* decryptedData = nullptr;
        int64_t decryptedDataLength = 0;
        result = xencrypt_service_decrypt(pContext, encryptedData, encryptedDataLength, &decryptedData, &decryptedDataLength);

        if (result != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for decryptedData.
        }
        
        xencrypt_release_xcontext(pContext);

        XMEMORY_SAFE_FREE(encryptedData);
        //stop service
        xencrypt_service_deinitialize();
    }
    getchar();
}