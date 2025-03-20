#pragma once

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

    /// @brief 创建XEF格式加密/解密器插件实例
    /// @param type 数据加密编码类型
    /// @param encryptSize 数据加密长度
    /// @return 插件实例指针
    XENCRYPT_API void* xef_plugin_create(int type, uint8_t encryptSize);
    /// @brief 销毁XEF格式 加密/解密插件实例 
    /// @param plugin 已创建的插件实例
    XENCRYPT_API void xef_plugin_destroy(void* plugin);

#if defined(__cplusplus)
}
#endif // __cplusplus