#pragma once

#include "config.h"

namespace xencrypt
{
    typedef void (*XLogCallback)(const char*);

    class Logging
    {
    public:
        static void Write(const char* format, ...);
        static void SetLogCallback(XLogCallback method);
        static bool IsLogCallbackSet();

    private:
        static XLogCallback s_Callback;
    };
}
