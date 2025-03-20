#pragma once

#include <stdint.h>
#include "config.h"
#include "plugin/Decoder.h"

namespace xencrypt
{
    /// @brief XEF格式解密器
    class XENCRYPT_API XEFDecoder final : public Decoder
    {
    public:
        XEFDecoder() = default;
        virtual ~XEFDecoder() = default;
        XEFDecoder(XEFDecoder&&) noexcept = default;
        XEFDecoder(const XEFDecoder&) = delete;
        
        XEFDecoder& operator=(const XEFDecoder&) = delete;
        /// @brief 解密数据
        /// @param context 解密器上下文
        virtual void Decode(XContext* context) final override;

    private:
        bool DecryptData(XContext* context, byte* data, byte** out, size_t& unpackedLen);
    };
}
