#pragma once

#include "config.h"

namespace encrypt
{
    typedef void (*XLogCallback)(const char*);

    class XENCRYPT_API Logging
    {
    public:
        static void Write(const char* format, ...);
        static void SetLogCallback(XLogCallback method);
        static bool IsLogCallbackSet();

    private:
        static XLogCallback s_Callback;
    };
}
