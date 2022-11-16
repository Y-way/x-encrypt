#pragma once

#include "XContext.h"

namespace encrypt
{
	class XENCRYPT_API Decoder
	{
	public:
        Decoder(){}
		virtual ~Decoder() {};
		virtual void Decode(XContext* context) = 0;
	};
}
