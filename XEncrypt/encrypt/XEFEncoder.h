#pragma once

#include <stdint.h>
#include "config.h"
#include "Encoder.h"

namespace encrypt
{
	/// <summary>
	/// XEF格式加密器
	/// </summary>
	class XENCRYPT_API XEFEncoder : public Encoder
	{
	public:
        XEFEncoder() {};
		virtual ~XEFEncoder() {};
        /// <summary>
        /// 加密处理
        /// </summary>
        /// <param name="context">加密上下文</param>
        /// <param name="encryptSize">加密数据长度</param>
        /// <param name="type">源数据加密编码样式</param>
		virtual void Encode(XContext* context, uint8_t encryptSize, XEncodeType type) X_ENCRYPT_FINAL;

    private:
        byte* EncryptData(byte* rawdata, int size);
	};
}
