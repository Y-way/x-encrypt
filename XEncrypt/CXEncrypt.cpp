// CXEncrypt.cpp: 定义应用程序的入口点。

#include "iostream"
#include "CXEncrypt.h"
#include "encrypt/service/XService.h"
#include "encrypt/service/Common.h"
#include "encrypt/RuntimeApi.h"
#include "encrypt/common/Allocator.h"

#include "encrypt/plugin/xef/XEncodeType.h"
#include "encrypt/plugin/xef/XEFPlugin.h"
#include "encrypt/plugin/xef/XEFRuntimeApi.h"

using namespace std;
using namespace xencrypt;

const byte rawData[] = { 'T','h','i','s',' ','a',' ','t','e','s','t',' ','d', 'a', 't', 'a', '\0', };
int64_t length = sizeof(rawData);

int main()
{
    {
        //C++
        XEFPlugin* plugin = new XEFPlugin(XEncodeType::XGZip, 32);
        
        //start service
        XService* service = new XService(plugin);
        //Encrypting data

        if(service->IsEncrypted(rawData, length))
        {
            std::cout<<"data has been encrypted."<<endl;
        }

        XResult* result = service->Encrypt(rawData, length);

        const byte* encryptedData = nullptr;

        int64_t encryptedDataLength = 0;
        if (result->GetCode() != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            encryptedDataLength = result->GetDataSize();
            //continue to do something for encryptedData.
            encryptedData = (const byte*)XMEMORY_MALLOC(encryptedDataLength);
            memcpy((void*)encryptedData, result->GetData(), encryptedDataLength);
        }
        
        service->ReleaseResult(result);
        result = nullptr;
        //Decrypting data
        if(!service->IsEncrypted(encryptedData, encryptedDataLength))
        {
            cout<<"data dose not have been encrypted."<<endl;
        }

        result = service->Decrypt(encryptedData, encryptedDataLength, true);

        if (result->GetCode() != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for decryptedData.
        }
        service->ReleaseResult(result);
        result = nullptr;

        XMEMORY_SAFE_FREE(encryptedData);

        //stop service
        delete service;
        service = nullptr;

        delete plugin;
        plugin = nullptr;
    }

    {
        //C API
        void* plugin = xef_plugin_create(XEncodeType::XGZip, 32);
        //start service
        void* service = xencrypt_service_initialize(plugin);

        //Encrypting data 
        if(xencrypt_service_is_encrypted(service, rawData, length))
        {
            printf("data has been encrypted.");
        }

        void* pEncryptBuff = nullptr;
        int64_t encryptedDataLength = 0;
        int code = 0;
        void* result = xencrypt_service_encrypt(service, rawData, length, &code, &pEncryptBuff, &encryptedDataLength);

        const byte* encryptedData = nullptr;
        if (code != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for encryptedData.
            encryptedData = (const byte*)XMEMORY_MALLOC(encryptedDataLength);
            memcpy((void*)encryptedData, pEncryptBuff, encryptedDataLength);
        }
        xencrypt_service_release_result(service, result);
        //Decrypting data
        if(!xencrypt_service_is_encrypted(service, encryptedData, length))
        {
            printf("data dose not have been encrypted.");
        }

        void* decryptedData = nullptr;
        int64_t decryptedDataLength = 0;
        result = xencrypt_service_decrypt(service, encryptedData, encryptedDataLength, &code, &decryptedData, &decryptedDataLength);

        if (code != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for decryptedData.
        }
        xencrypt_service_release_result(service, result);

        XMEMORY_SAFE_FREE(encryptedData);

        //stop service
        xencrypt_service_deinitialize(service);

        xef_plugin_destroy(plugin);
    }
    getchar();
}
