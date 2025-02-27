#pragma once

#include "XContext.h"

namespace xencrypt
{
    /// <summary>
    /// 解密器接口
    /// </summary>
    class XENCRYPT_API Decoder
    {
    public:
        Decoder(){}
        virtual ~Decoder() {};
        /// <summary>
        /// 解密数据
        /// </summary>
        /// <param name="context">解密器上下文</param>
        virtual void Decode(XContext* context) = 0;
    };
}
