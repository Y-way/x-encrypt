# x-encrypt
简单,高效,跨平台的自定义加密/解密库.<br/>
支持自定义的加密/解密器

### 支持系统
|系统|支持||
|---------------|----------------------|-------|
|Windows|x86<br/>x86_64<br/>|[✔]<br/>[✔]<br/>|
|Linux|x86_64<br/>|[✔]<br/>|
|osx|x86_64<br/>|[✔]<br/>|
|Android|armeabi-v7a <br/>arm64-v8a<br/>x86<br/>x86_64<br/>|[✔]<br/>[✔]<br/>[✔]<br/>[✔]<br/>|
|iOS|arm64|[✔]<br/>|
|tvOS|arm64|[✔]<br/>|
|UWP|x86<br/>x86_64<br/>arm64<br/>|[✔❗未测试]<br/>[✔❗未测试]<br/>[✔❗未测试]<br/>|


## 使用CMake编译x-Encrypt的测试程序和示例程序
```sh
git clone --recursive https://github.com/Y-way/x-encrypt
cd x-encrypt

# 如果是macOS Xcode, 这里命令应该换成：cmake -B build -GXcode
cmake -B build -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_ARCHITECTURES=arm64 -DPLAT=iOS -DDEPLOYMENT_TARGET=11.0

# 使用CMake命令行编译, 如果需要调试，则使用相应平台IDE打开即可:
# a. Windows：使用VisualStudio打开build/CXEncrypt.sln
# b. macOS：使用Xcode打开build/CXEncrypt.xcodeproj
cmake --build build --config Debug

# # 者直接用VS打开 
```

## CMake 编译说明

|编译宏|值|默认|说明|
|---------------|----------------------|----|--------|
|SUPPORT_ENCRYPT|ON:支持 <br /> OFF:关闭|ON  |支持加密|
|SUPPORT_DECRYPT|ON:支持 <br /> OFF:关闭|ON  |支持解密|
|XENCRYPT_TESTS |ON:支持 <br /> OFF:关闭|OFF |测试用例|

#### 注:
   `SUPPORT_ENCRYPT`和`SUPPORT_DECRYPT`必须至少开启其中一个

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

        byte* encryptedData = nullptr;

        int64_t encryptedDataLength = 0;
        if (result->GetCode() != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            encryptedDataLength = result->GetDataSize();
            //continue to do something for encryptedData.
            encryptedData = (byte*)XMEMORY_MALLOC(encryptedDataLength);
            memcpy((void*)encryptedData, result->GetData(), encryptedDataLength);
        }
        
        service->ReleaseResult(result);
        result = nullptr;

        //Decrypting data
        if(!service->IsEncrypted(encryptedData, encryptedDataLength))
        {
            cout<<"data dose not have been encrypted."<<endl;
        }
        byte* decryptedData = nullptr;
        result = service->Decrypt(encryptedData, encryptedDataLength, true);

        if (result->GetCode() != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for decryptedData.
            decryptedData = (byte*)XMEMORY_MALLOC(result->GetDataSize());
            memcpy((void*)decryptedData, result->GetData(), result->GetDataSize());
        }
        service->ReleaseResult(result);
        result = nullptr;

        //stop service
        delete service;
        service = nullptr;

        delete plugin;
        plugin = nullptr;

        XMEMORY_SAFE_FREE(encryptedData);

        XMEMORY_SAFE_FREE(decryptedData);
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

        xencrypt_result result = xencrypt_service_encrypt(service, rawData, length);
        byte* encryptedData = nullptr;
        int64_t encryptedDataLength = result.size;
        if (result.code != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for encryptedData.
            encryptedData = (byte*)XMEMORY_MALLOC(result.size);
            memcpy((void*)encryptedData, result.data, result.size);
        }
        xencrypt_service_release_result(service, &result);
        //Decrypting data
        if(!xencrypt_service_is_encrypted(service, encryptedData, length))
        {
            printf("data dose not have been encrypted.");
        }

        byte* decryptedData = nullptr;

        result = xencrypt_service_decrypt(service, encryptedData, encryptedDataLength);

        if (result.code != ResultCode::Ok)
        {
            //todo something.
        }
        else
        {
            //continue to do something for decryptedData.
            decryptedData = (byte*)XMEMORY_MALLOC(result.size);
            memcpy((void*)decryptedData, result.data, result.size);
        }

        xencrypt_service_release_result(service, &result);
        
        //stop service
        xencrypt_service_deinitialize(service);
        service = nullptr;

        xef_plugin_destroy(plugin);
        plugin = nullptr;

        XMEMORY_SAFE_FREE(decryptedData);
        XMEMORY_SAFE_FREE(encryptedData);
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

[XEF加密格式说明](./XEFFormat.md).
-----

# XFileEncoder
 
 C#导出API使用`x-encrypt`库进行加密/解密

 # 使用方法

使用 Visual Studio 打开`./C#/XFileEncoder.sln`工程

编译并测试

打开`C#/XFileEncoder/Properties/launchSettings.json`文件

修改`commandLineArgs` 命令行参数测试加密/解密功能

```
Usage:

XFileEncoder command args

    command:执行命令,必须参数
        encrypt:加密命令
        decrypt:解密命令
    args:命令参数
        -load:加载欲加密文件,必须参数
        -out:输出文件名字.可选参数,默认文件名out.xfe
        -encrypt-size:加密数据长度,可选参数,默认16字节. 取值范围:Min(Clamp(encrypt-size, 1, 255), file_size)
        -encode-type:加密源数据方式,可选参数. none:只加密源文件内容, 默认;zip:加密并zip压缩源文件内容
        -help:查看帮助
    例:
    加密:
        XFileEncoder encrypt -load test.png -out test.png -encrypt-size 32 -encode-type zip
    解密:
        XFileEncoder edcrypt -load test.png -out test.png

```

# Unity插件
基于`x-encrypt`库和C#`XFileEncoder`,实现的Unity插件.

项目地址:[XEncrypt-Unity](https://github.com/Y-way/XEncrypt-Unity)