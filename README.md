# xencrypt
一个简单的自定义加密/解密库.
支持自定义的加密/解密器


## 使用CMake编译xencrypt的测试程序和示例程序
```sh
git clone --recursive https://github.com/Y-way/xencrypt
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
        #include "CXEncrypt.h"
        #include "encrypt/XService.h"
        #include "encrypt/XContext.h"
        #include "encrypt/common/Common.h"
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

                XContext* pContext = XService::CreateContext(XContextType::Encrypt);

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


                pContext = XService::CreateContext(XContextType::Decrypt);
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
                decrypt_service_initialize();

                //Encrypting data
                // 
                // if(decrypt_service_is_encrypted(data, length))
                // {
                //     printf("data has been encrypted.");
                // }

                void* pContext = decrypt_create_xcontext(XContextType::Encrypt);

                void* pEncryptBuff = nullptr;
                int64_t encryptedDataLength = 0;

                int result = decrypt_service_encrypt(pContext, rawData, length, &pEncryptBuff, &encryptedDataLength, 32, 1);

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

                decrypt_release_xcontext(pContext);


                //Decrypting data

                // if(!decrypt_service_is_encrypted(data, length))
                // {
                //     printf("data dose not have been encrypted.");
                // }


                pContext = decrypt_create_xcontext(XContextType::Decrypt);

                void* decryptedData = nullptr;
                int64_t decryptedDataLength = 0;
                result = decrypt_service_decrypt(pContext, encryptedData, encryptedDataLength, &decryptedData, &decryptedDataLength);

                if (result != ResultCode::Ok)
                {
                    //todo something.
                }
                else
                {
                    //continue to do something for decryptedData.
                }
                
                decrypt_release_xcontext(pContext);

                XMEMORY_SAFE_FREE(encryptedData);
                //stop service
                decrypt_service_deinitialize();
            }
            getchar();
        }
    ```