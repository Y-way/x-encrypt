#pragma once

#include "service/XContext.h"

namespace xencrypt
{
    /// @brief 加密器
    class XENCRYPT_API Encoder
    {
    public:
        /// @brief 加密处理
        /// @param context 加密上下文
        virtual void Encode(XContext* context) = 0;
    };
}
