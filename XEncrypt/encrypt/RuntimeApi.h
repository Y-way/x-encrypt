#pragma once

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

#define EXPORT_API(r, n, p)             X_ENCRYPT_EXPORT r n p;
    /// <summary>
    /// 初始化服务
    /// </summary>
    EXPORT_API(void, xencrypt_service_initialize, ());
    /// <summary>
    /// 注销服务
    /// </summary>
    EXPORT_API(void, xencrypt_service_deinitialize, ());
    /// <summary>
    /// 创建指定类型的服务上下文
    /// </summary>
    /// <returns>指定类型的服务上下文</returns>
    EXPORT_API(void*, xencrypt_create_xcontext, (int type));
    /// <summary>
    /// 判定数据是否已加密
    /// </summary>
    /// <returns>数据已加密,返回true, 否则返回false</returns>
    EXPORT_API(bool, xencrypt_service_is_encrypted, (const byte* in, int64_t in_size));
    /// <summary>
    /// 加密数据
    /// </summary>
    /// <param name="context">加密服务上下文</param>
    /// <param name="in">待加密数据</param>
    /// <param name="in_size">待加密数据长度</param>
    /// <param name="out">指向加密输出数据缓存的指针</param>
    /// <param name="out_size">加密后数据长度</param>
    /// <param name="encryptSize">加密长度</param>
    /// <param name="encryptSize">源数据加密编码样式</param>
    EXPORT_API(int, xencrypt_service_encrypt, (void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size, uint8_t encryptSize, int type));
    /// <summary>
    /// 解密数据
    /// </summary>
    /// <param name="context">解密服务上下文</param>
    /// <param name="in">加密数据</param>
    /// <param name="in_size">加密数据长度</param>
    /// <param name="in_size">指向解密输出数据缓存的指针</param>
    /// <param name="out_size">解密后数据长度</param>
    EXPORT_API(int, xencrypt_service_decrypt, (void* context, const byte* in, int64_t in_size, void** out, int64_t* out_size));
    /// <summary>
    /// 销毁服务上下文,并释放上下文创建的缓存数据
    /// </summary>
    /// <param name="context">服务上下文</param>
    EXPORT_API(void, xencrypt_release_xcontext, (void* context));

 #undef EXPORT_API
#if defined(__cplusplus)
}
#endif // __cplusplus
