#pragma once

#include "config.h"
#include "Encoder.h"
#include "Decoder.h"

namespace xencrypt
{
    /// @brief 加密/解密器插件抽象基类
    class XENCRYPT_API XPlugin
    {
    public:
        XPlugin():_encoder(nullptr), _decoder(nullptr) {}
		virtual ~XPlugin() {}

    public:
        /// @brief 根据XContext上下文类型,判定当前插件是否支持
        /// @param type 加密/解密上下文类型
        /// @return 返回true,支持该类型;否则,不支持.
        bool IsSupport(XContextType type) const;
        
    public:
        /// @brief 加密抽象接口
        /// @param context 上下文实例
        virtual void Encrypt(XContext* context) = 0;
        /// @brief 解密抽象接口
        /// @param context 上下文实例
        virtual void Decrypt(XContext* context) = 0;
        /// @brief 检查数据是否已加密
        /// @param data 内存数据地址
        /// @param size 数据长度
        /// @return 数据是否已加密.返回true,数据已加密,否则,未加密.
        virtual bool IsEncrypted(const byte* data, int64_t size) = 0;

    protected:
        Encoder* _encoder;
        Decoder* _decoder;
    };

    inline bool XPlugin::IsSupport(XContextType type) const
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
