# x-Encrypt
一个简单的自定义加密/解密库.
支持自定义的加密/解密器


## 使用CMake编译x-Encrypt的测试程序和示例程序
```sh
git clone --recursive https://github.com/Y-way/x-Encrypt
cd xencrypt

# 如果是macOS Xcode, 这里命令应该换成：cmake -B build -GXcode
cmake -B build

# 使用CMake命令行编译, 如果需要调试，则使用相应平台IDE打开即可:
# a. Windows：使用VisualStudio打开build/CXEncrypt.sln
# b. macOS：使用Xcode打开build/CXEncrypt.xcodeproj
cmake --build build --config Debug

# # 者直接用VS打开 
```

## 快速开始
```cpp
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
        if(!service->IsEncrypted(encryptedData, length))
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
        void* plugin = xefencrypt_plugin_create(XEncodeType::XGZip, 32);
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

        xefencrypt_plugin_destroy(plugin);
    }
    getchar();
}
```
## 扩展自定义加密/解密器.
    1)实现'XPlugin'插件接口类
    2)实现'Encoder'编码接口类,用于支持数据加密
    3)实现'Decoder'解码接口类,用于支持数据解密.
    4)实现'IsEncrypted'判定是否加密接口,用于判定数据是否加密.

## 参考 'encrypt/plugin/xef/'目录下 XEF格式文件(数据)加密,解密.
    'XEFEncoder'类实现'Encoder'接口类,用于XEF格式数据加密.
    'XEFDecoder'类实现'Decoder'接口类,用于XEF格式数据解密.
    'XEFPlugin'类实现'XPlugin'插件接口类,用于注册到XService.
