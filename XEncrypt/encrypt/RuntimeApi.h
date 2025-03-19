#pragma once

#include "config.h"

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus
    /// @brief 加密/解密结果
    typedef struct xencrypt_result
    {
        /// @brief 结果码
        int code;
        /// @brief 结果数据大小
        int64_t size;
        /// @brief 结果数据
        byte* data;
        /// @brief XReult指针, 用于释放内存
        void* result;
    }xencrypt_result;

#define EXPORT_API(r, n, p)             XENCRYPT_API r n p;
    /// @brief 初始化服务
    /// @param plugin 加密/解密插件实例
    EXPORT_API(void*, xencrypt_service_initialize, (void* plugin));
    /// @brief 检查数据是否已加密
    /// @param service 加密/解密服务实例
    /// @param data 内存数据地址
    /// @param size 数据长度
    /// @return 数据是否已加密.返回true,数据已加密,否则,未加密.
    EXPORT_API(bool, xencrypt_service_is_encrypted, (void* service, const byte* data, int64_t size));
    /// @brief 加密数据
    /// @param service 加密/解密服务实例
    /// @param in 待加密数据
    /// @param in_size 待加密数据长度
    /// @return 加密结果
    EXPORT_API(xencrypt_result, xencrypt_service_encrypt, (void* service, const byte* in, int64_t in_size));
    /// @brief 解密数据
    /// @param service 加密/解密服务实例
    /// @param in 待解密数据
    /// @param in_size 密数据长度
    /// @return 解密结果
    EXPORT_API(xencrypt_result, xencrypt_service_decrypt, (void* service, const byte* in, int64_t in_size));
    /// @brief 销毁结果
    /// @param service 加密/解密服务实例
    /// @param result 加/解密结果指针
    EXPORT_API(void, xencrypt_service_release_result, (void* service, xencrypt_result* result));
    /// @brief 注销服务
    /// @param service 加密/解密服务实例
    EXPORT_API(void, xencrypt_service_deinitialize, (void* service));
 #undef EXPORT_API
#if defined(__cplusplus)
}
#endif // __cplusplus
