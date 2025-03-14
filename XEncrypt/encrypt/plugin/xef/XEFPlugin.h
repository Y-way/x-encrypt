#pragma once


#include "plugin/XPlugin.h"
#include "XEncodeType.h"

namespace xencrypt
{
    /// @brief XEF 格式加密/解密器插件
    class XENCRYPT_API XEFPlugin : public XPlugin
    {
    public:
        XEFPlugin(XEncodeType type, uint8_t encryptSize);
        ~XEFPlugin();
        XEFPlugin(const XEFPlugin&) = delete;
        XEFPlugin(XEFPlugin&&) = delete;
        XEFPlugin& operator=(const XEFPlugin&) = delete;
    public:
        /// @brief 检查数据是否已加密
        /// @param data 内存数据地址
        /// @param size 数据长度
        /// @return 数据是否已加密.返回true,数据已加密,否则,未加密.
        virtual bool IsEncrypted(const byte* data, int64_t size);
        /// @brief 加密
        /// @param context 加密上下文
        virtual void Encrypt(XContext* context) final;
        /// @brief 解密
        /// @param context 解密上下文
        virtual void Decrypt(XContext* context)  final;
    };
}