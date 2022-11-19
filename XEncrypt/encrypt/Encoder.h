#pragma once

#include "XContext.h"

namespace encrypt
{
    /// <summary>
    /// 加密器
    /// </summary>
    class XENCRYPT_API Encoder
    {
    public:
        Encoder() {}
        virtual ~Encoder() {};
        /// <summary>
        /// 加密处理
        /// </summary>
        /// <param name="context">加密上下文</param>
        /// <param name="encryptSize">加密数据长度</param>
        /// <param name="type">源数据加密编码样式</param>
        virtual void Encode(XContext* context, uint8_t encryptSize, XEncodeType type) = 0;
    };
}
