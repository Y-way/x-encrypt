#pragma once

#include <stdint.h>
#include "config.h"
#include "Decoder.h"

namespace encrypt
{
	class XENCRYPT_API XEFDecoder : public Decoder
	{
	public:
        XEFDecoder() {};
		virtual ~XEFDecoder() {};

		virtual void Decode(XContext* context) X_ENCRYPT_FINAL;

    private:
        byte* DecryptData(byte* rawdata, int size);
	};
}
