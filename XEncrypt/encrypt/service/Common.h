#pragma once

namespace xencrypt
{
    /// <summary>
    /// 服务上下文处理输出数据时,使用的内存缓存类型
    /// </summary>
    enum XCodeMemoryType
    {
        None,
        /// <summary>
        /// 申请新内存
        /// </summary>
        AllocateMemory,
        /// <summary>
        /// 源数据内存偏移
        /// </summary>
        OriginalOffset,
    };

    /// <summary>
    /// 服务上下文类型
    /// </summary>
    enum XContextType
    {
        /// <summary>
        /// 未知
        /// </summary>
        XUnknown,
        /// <summary>
        /// 加密上下文
        /// </summary>
        XEncrypt,
        /// <summary>
        /// XEF解密上下文
        /// </summary>
        XDecrypt,
    };
    /// <summary>
    /// 服务上下文处理数据结构状态
    /// </summary>
    enum ResultCode
    {
        /// <summary>
        /// 完成
        /// </summary>
        Ok,
        /// <summary>
        /// 未知
        /// </summary>
        Unknown,
        /// <summary>
        /// 服务未初始化
        /// </summary>
        UnInitialize,
        /// <summary>
        /// 无效的输入数据
        /// </summary>
        InvalidInputData,
        /// <summary>
        /// 解密数据长度错误
        /// </summary>
        InvalidInputDataSize,
        /// <summary>
        /// 无效的解密器
        /// </summary>
        InvalidDecoder,
        /// <summary>
        /// 解密数据解压缩失败
        /// </summary>
        InvalidUnzip,
        /// <summary>
        /// 无效的加密器
        /// </summary>
        InvalidEncoder,
        /// <summary>
        /// 数据GZip压缩编码失败
        /// </summary>
        InvalidZip,
        /// <summary>
        /// 数据已加密
        /// </summary>
        EncryptedData,
        /// <summary>
        /// 内存不足
        /// </summary>
        OutMemory,
        /// <summary>
        /// 服务上下文类型不匹配
        /// </summary>
        ContextTypeError,
        /// <summary>
        /// 不支持解密服务
        /// </summary>
        NotSupportDecrypt,
        /// <summary>
        /// 不支持加密服务
        /// </summary>
        NotSupportEncrypt,
    };
}
