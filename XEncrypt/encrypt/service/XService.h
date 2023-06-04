#pragma once

#include <stdint.h>

#include "config.h"
#include "Common.h"

#include "Decoder.h"
#include "Encoder.h"
#include "XContext.h"
#include "plugin/xef/XEFConsts.h"

namespace xencrypt
{

    class XENCRYPT_API XService
    {
    public:
        /// <summary>
        /// 数据是否已加密
        /// </summary>
        /// <param name="data"></param>
        /// <param name="size"></param>
        /// <returns></returns>
        static bool IsEncrypted(const byte* data, int64_t size);
        /// <summary>
        /// 初始化服务
        /// </summary>
        static void Initialize();
        /// <summary>
        /// 注销服务
        /// </summary>
        static void UnInitialize();
        /// <summary>
        /// 使用解密上下文进行数据解密处理
        /// </summary>
        /// <param name="context">解密类型上下文</param>
        /// <param name="in">加密数据</param>
        /// <param name="length">输入数据长度</param>
        /// <param name="cloneInput">输入数据是否要内存复制</param>
        /// <returns>返回解密状态码</returns>
        static ResultCode Decrypt(XContext* context, const byte* in, int64_t length, bool cloneInput = false);
        /// <summary>
        /// 使用加密上下文进行数据加密
        /// </summary>
        /// <param name="context">加密类型上下文</param>
        /// <param name="in">待加密数据</param>
        /// <param name="length">数据长度</param>
        /// <param name="encryptSize">加密长度</param>
        /// <param name="type">待加密数据的编码样式</param>
        /// <returns>返回加密状态码</returns>
        static ResultCode Encrypt(XContext* context, const byte* in, int64_t length, uint8_t encryptSize, XEncodeType type);

        /// <summary>
        /// 创建指定类型的服务上下文
        /// </summary>
        /// <param name="type">上下文类型</param>
        /// <returns>指定类型的服务上下文</returns>
        static XContext* CreateContext(XContextType type);
        /// <summary>
        /// 销毁服务创建的上下文,并销毁有上下文持有的内存缓存数据
        /// </summary>
        /// <param name="context"></param>
        static void ReleaseContext(XContext* context);
    public:
        /// <summary>
        /// 注册指定类型的解密器
        /// </summary>
        /// <typeparam name="T"></typeparam>
        template<typename T>
        void RegisterDecoder();
        /// <summary>
        /// 销毁已注册的解密器
        /// </summary>
        void UnregisterDecoder();
        /// <summary>
        /// 注册指定类型的加密器
        /// </summary>
        /// <typeparam name="T"></typeparam>
        template<typename T>
        void RegisterEncoder();
        /// <summary>
        /// 销毁指定类型的加密器
        /// </summary>
        void UnregisterEncoder();

        ~XService();

    private:
        XService() :_decoder(nullptr) {}
        XService(const XService&):_decoder(nullptr) {}
        XService& operator=(const XService&);

    private:
        /// <summary>
        /// 解密器接口
        /// </summary>
        Decoder* _decoder;
        /// <summary>
        /// 加密器接口
        /// </summary>
        Encoder* _encoder;
    };

    template<typename T>
    void XService::RegisterDecoder()
    {
        UnregisterDecoder();
        _decoder = new T();
    }

    template<typename T>
    void XService::RegisterEncoder()
    {
        UnregisterDecoder();
        _encoder = new T();
    }
}
