// CXEncrypt.cpp: 定义应用程序的入口点。
//

#include "CXEncrypt.h"
#include "encrypt/service/XService.h"
#include "encrypt/service/XContext.h"
#include "encrypt/service/Common.h"
#include "encrypt/RuntimeApi.h"
#include "encrypt/common/Allocator.h"

#include "encrypt/RuntimeApi.h"

#include "encrypt/plugin/xef/XEFHeader.h"
#include "encrypt/plugin/xef/XEncodeType.h"
#include "encrypt/plugin/xef/XEFEncryptPlugin.h"
#include "encrypt/plugin/xef/XEFRuntimeApi.h"

using namespace std;
using namespace xencrypt;

const byte rawData[] = { 'T','h','i','s',' ','a',' ','t','e','s','t',' ','d', 'a', 't', 'a', '\0', };
int64_t length = sizeof(rawData);

int main()
{
    {
        //C++
        XEFEncryptPlugin* plugin = new XEFEncryptPlugin(XEncodeType::XGZip, 32);
        

        //Encrypting data
        
        //if(XEFEncryptPlugin::IsEncrypted(rawData, length))
        //{
        //    cout>>"data has been encrypted."<<endl;
        //}
        
        //start service
        XService::Initialize(plugin);

        XContext* pContext = XService::CreateContext(XContextType::XEncrypt);

        ResultCode result = XService::Encrypt(pContext, rawData, length);

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

        // if(!XEFEncryptPlugin::IsEncrypted(encryptedData, length))
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

        delete plugin;
        plugin = nullptr;
    }

    {
        //C API
        void* plugin = xefencrypt_plugin_create(XEncodeType::XGZip, 32);
        //start service
        xencrypt_service_initialize(plugin);

        //Encrypting data
        // 
        // if(xefencrypt_is_encrypted(data, length))
        // {
        //     printf("data has been encrypted.");
        // }

        void* pContext = xencrypt_create_xcontext(XContextType::XEncrypt);

        void* pEncryptBuff = nullptr;
        int64_t encryptedDataLength = 0;

        int result = xencrypt_service_encrypt(pContext, rawData, length, &pEncryptBuff, &encryptedDataLength);

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

        // if(!xefencrypt_is_encrypted(data, length))
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

        xefencrypt_plugin_destroy(plugin);
    }
    getchar();
}
