#pragma once

#include <stdint.h>
#include "config.h"
#include "Decoder.h"

namespace encrypt
{
	/// <summary>
	/// XEF格式解密器
	/// </summary>
	class XENCRYPT_API XEFDecoder : public Decoder
	{
	public:
        XEFDecoder() {};
		virtual ~XEFDecoder() {};
        /// <summary>
        /// 解密数据
        /// </summary>
        /// <param name="context">解密器上下文</param>
		virtual void Decode(XContext* context) X_ENCRYPT_FINAL;

    private:
        bool DecryptData(XContext* context, byte* data, byte** out, size_t& unpackedLen);
	};
}
