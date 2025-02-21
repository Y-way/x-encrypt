#pragma once

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

#define EXPORT_API(r, n, p)             XENCRYPT_API r n p;
    /// @brief 初始化服务
    /// @param plugin 加密/解密插件实例
    EXPORT_API(void, xencrypt_service_initialize, (void* plugin));
    /// @brief 注销服务
    EXPORT_API(void, xencrypt_service_deinitialize, ());
    /// @brief 重新注册加密/解密插件
    /// @param plugin 加密/解密插件实例
    /// @return 旧的插件实例
    EXPORT_API(void*, xencrypt_service_register_plugin, (void* plugin));
    /// @brief 创建指定类型的服务上下文
    /// @param type 指定类型的服务上下文
    EXPORT_API(void*, xencrypt_create_xcontext, (int type));
    /// @brief 加密数据
    /// @param context 加密服务上下文
    /// @param in 待加密数据
    /// @param in_size 待加密数据长度
    /// @param out 指向加密输出数据缓存的指针
    /// @param out_size 加密后数据长度
    EXPORT_API(int, xencrypt_service_encrypt, (void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size));
    /// @brief 解密数据
    /// @param context 解密服务上下文
    /// @param in 待解密数据
    /// @param in_size 密数据长度
    /// @param out 指向解密输出数据缓存的指针
    /// @param out_size 解密后数据长度
    EXPORT_API(int, xencrypt_service_decrypt, (void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size));
    /// @brief 销毁上下文,并释放上下文创建的缓存数据
    /// @param context 加密/解密上下文
    EXPORT_API(void, xencrypt_release_xcontext, (void* context));

 #undef EXPORT_API
#if defined(__cplusplus)
}
#endif // __cplusplus
