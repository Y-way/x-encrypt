#pragma once

#include "config.h"
#include "Encoder.h"
#include "Decoder.h"

namespace xencrypt
{
    /// @brief 加密/解密器插件抽象基类
    class XENCRYPT_API XEncryptPlugin
    {
    public:
        XEncryptPlugin():_encoder(nullptr), _decoder(nullptr) {}
		virtual ~XEncryptPlugin() {}

    public:
        /// @brief 根据XContext上下文类型,判定当前插件是否支持
        /// @param type 加密/解密上下文类型
        /// @return 返回true,支持该类型;否则,不支持.
        bool IsSupport(XContextType type);
        
    public:
        /// @brief 加密抽象接口
        /// @param context 上下文实例
        virtual void Encrypt(XContext* context) = 0;
        /// @brief 解密抽象接口
        /// @param context 上下文实例
        virtual void Dencrypt(XContext* context) = 0;

    protected:
        Encoder* _encoder;
        Decoder* _decoder;
    };

    inline bool XEncryptPlugin::IsSupport(XContextType type)
    {
        bool support = false;
        if (type == XContextType::XDecrypt)
        {
            support = _decoder != nullptr;
        }
        else if (type == XContextType::XEncrypt)
        {
            support = _encoder != nullptr;
        }
        return support;
    }
}
