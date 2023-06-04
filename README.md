# xEncrypt
一个简单的自定义加密/解密库.
支持自定义的加密/解密器


## 使用CMake编译xEncrypt的测试程序和示例程序
```sh
git clone --recursive https://github.com/Y-way/xEncrypt
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

=== "C++"

    ```cpp
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
    ```

## 扩展自定义加密/解密器.
    1)实现'XEncryptPlugin'插件接口类
    2)实现'Encoder'编码接口类,用于支持数据加密
    3)实现'Decoder'解码接口类,用于支持数据解密.

## 参考 'encrypt/plugin/xef/'目录下 XEF格式文件(数据)加密,解密.
    'XEFEncoder'类实现'Encoder'接口类,用于XEF格式数据加密.
    'XEFDecoder'类实现'Decoder'接口类,用于XEF格式数据解密.
    'XEFEncryptPlugin'类实现'XEncryptPlugin'插件接口类,用于注册到XService.
