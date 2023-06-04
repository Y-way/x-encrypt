#include "XEFRuntimeApi.h"
#include "XEFEncryptPlugin.h"

using namespace xencrypt;

bool xefencrypt_is_encrypted(const byte* in, int64_t in_size)
{
    return XEFEncryptPlugin::IsEncrypted(in, in_size);
}

void* xefencrypt_plugin_create(int type, uint8_t encryptSize)
{
    return new XEFEncryptPlugin((XEncodeType)type, encryptSize);
}

void xefencrypt_plugin_destroy(void* plugin)
{
    XEFEncryptPlugin* pluginPtr = reinterpret_cast<XEFEncryptPlugin*>(plugin);
    if (pluginPtr == nullptr)
    {
        return;
    }
    delete pluginPtr;
    pluginPtr = nullptr;
}