#pragma once

#include <stdint.h>
#include "config.h"
#include "Encoder.h"

namespace encrypt
{
	class XENCRYPT_API XEFEncoder : public Encoder
	{
	public:
        XEFEncoder() {};
		virtual ~XEFEncoder() {};

		virtual void Encode(XContext* context, uint8_t encryptSize, XEncodeType type) X_ENCRYPT_FINAL;

    private:
        byte* EncryptData(byte* rawdata, int size);
	};
}