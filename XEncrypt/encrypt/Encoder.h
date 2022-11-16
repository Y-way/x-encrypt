#pragma once

#include "XContext.h"

namespace encrypt
{
    class XENCRYPT_API Encoder
    {
    public:
        Encoder() {}
        virtual ~Encoder() {};
        virtual void Encode(XContext* context, uint8_t encryptSize, XEncodeType type) = 0;
    };
}
