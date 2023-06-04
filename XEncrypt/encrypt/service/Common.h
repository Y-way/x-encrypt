#pragma once

namespace xencrypt
{
    /// @brief 服务上下文处理输出数据时,使用的内存缓存类型
    enum XCodeMemoryType
    {
        None,
        /// @brief 申请新内存
        AllocateMemory,
        /// @brief 源数据内存偏移
        OriginalOffset,
    };
    /// @brief 服务上下文类型
    enum XContextType
    {
        /// @brief 未知
        XUnknown,
        /// @brief 加密上下文
        XEncrypt,
        /// @brief 解密上下文
        XDecrypt,
    };
    /// @brief 服务上下文处理数据结构状态
    enum ResultCode
    {
        /// @brief 完成
        Ok,
        /// @brief 未知
        Unknown,
        /// @brief 服务未初始化
        UnInitialize,
        /// @brief 无效的加密/解密插件
        InvalidPlugin,
        /// @brief 无效的输入数据
        InvalidInputData,
        /// @brief 无效的上下文
        InvalidXContext,
        /// @brief 解密数据长度错误
        InvalidInputDataSize,
        /// @brief 无效的解密器
        InvalidDecoder,
        /// @brief 解密数据解压缩失败
        InvalidUnzip,
        /// @brief 无效的加密器
        InvalidEncoder,
        /// @brief 数据GZip压缩编码失败
        InvalidZip,
        /// @brief 数据已加密
        EncryptedData,
        /// @brief 内存不足
        OutMemory,
        /// @brief 服务上下文类型不匹配
        ContextTypeError,
        /// @brief 不支持解密服务
        NotSupportDecrypt,
        /// @brief 不支持加密服务
        NotSupportEncrypt,
    };
}
