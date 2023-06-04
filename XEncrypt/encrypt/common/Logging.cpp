#include <stdarg.h>
#include <cstdio>
#include <cassert>
#include "Logging.h"

using namespace encrypt;

static void DefaultLogCallback(const char* message)
{
    printf("%s\n", message);
}

XLogCallback Logging::s_Callback = DefaultLogCallback;

void Logging::Write(const char* format, ...)
{
    X_ENCRYPT_ASSERT(s_Callback != NULL);

    if (format == NULL)
        return;

    va_list va;
    va_start(va, format);

    const char* prefix = "[xfe] ";
    const int bufferSize = 1024 * 5;
    char buffer[bufferSize];
    memcpy(buffer, prefix, 6);
    vsnprintf(buffer + 6, bufferSize - 6, format, va);
    if (s_Callback != NULL)
    {
        s_Callback(buffer);
    }
    va_end(va);
}

void Logging::SetLogCallback(XLogCallback method)
{
    s_Callback = method;
}

bool Logging::IsLogCallbackSet()
{
    return s_Callback != DefaultLogCallback;
}
