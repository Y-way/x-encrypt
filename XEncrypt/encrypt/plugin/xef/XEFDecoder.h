#pragma once

#include <stdint.h>
#include "config.h"
#include "plugin/Decoder.h"

namespace xencrypt
{
    /// @brief XEF格式解密器
    class XENCRYPT_API XEFDecoder : public Decoder
    {
    public:
        XEFDecoder() {};
        virtual ~XEFDecoder() {};
        /// @brief 解密数据
        /// @param context 解密器上下文
        virtual void Decode(XContext* context) X_ENCRYPT_FINAL;

    private:
        bool DecryptData(XContext* context, byte* data, byte** out, size_t& unpackedLen);
    };
}
