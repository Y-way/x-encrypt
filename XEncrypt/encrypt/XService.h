#pragma once

#include <stdint.h>
#include "common/Common.h"

#include "Decoder.h"
#include "Encoder.h"
#include "XContext.h"

namespace encrypt
{

    class XENCRYPT_API XService
    {
    public:
        static bool IsEncrypted(const byte* data, int64_t size);

        static void Initialize();
        
        static void UnInitialize();
        static ResultCode Decrypt(XContext* context, const byte* in, int64_t length);
        
        static ResultCode Encrypt(XContext* context, const byte* in, int64_t length, uint8_t encryptSize, XEncodeType type);

        static XContext* CreateContext(XContextType type);

        static void ReleaseContext(XContext* context);


    public:
        template<typename T>
        void RegisterDecoder();
        void UnregisterDecoder();
        template<typename T>
        void RegisterEncoder();
        void UnregisterEncoder();

        ~XService();

    private:
        XService() :_decoder(nullptr) {}
        XService(const XService&):_decoder(nullptr) {}
        XService& operator=(const XService&);

    private:
        Decoder* _decoder;
        Encoder* _encoder;
    };

    template<typename T>
    void XService::RegisterDecoder()
    {
        UnregisterDecoder();
        _decoder = new T();
    }

    template<typename T>
    void XService::RegisterEncoder()
    {
        UnregisterDecoder();
        _encoder = new T();
    }
}
