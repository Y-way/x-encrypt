#pragma once

#include <stdint.h>
#include "config.h"
#include "plugin/Encoder.h"
#include "XEncodeType.h"

namespace xencrypt
{
    /// @brief XEF格式加密器
    class XENCRYPT_API XEFEncoder final : public Encoder
    {
    public:
        XEFEncoder(XEncodeType encodeType, uint8_t encryptSize):_encodeType(encodeType),_encryptSize(encryptSize){};
        virtual ~XEFEncoder() = default;
        XEFEncoder (XEFEncoder&&) noexcept = default;
        XEFEncoder (const XEFEncoder&) = delete;
        XEFEncoder& operator=(XEFEncoder&&) = delete;
        /// @brief 加密处理
        /// @param context 加密上下文
        virtual void Encode(XContext* context) final override;

    private:
        byte* EncryptData(byte* rawdata, int size);
        /// @brief 加密数据长度
        uint8_t _encryptSize;
        /// @brief 加密编码类型
        XEncodeType _encodeType;
    };
}
