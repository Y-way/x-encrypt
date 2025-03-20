#pragma once

#include "service/XContext.h"

namespace xencrypt
{
	/// @brief 解密器接口
	class XENCRYPT_API Decoder
	{
	public:
		/// @brief 解密数据
		/// @param context 解密器上下文
		virtual void Decode(XContext* context) = 0;
	};
}
