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
        Encrypt,
        /// <summary>
        /// 解密上下文
        /// </summary>
        Decrypt,
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
    /// <summary>
    /// 加密服务时,源数据加密编码格式
    /// </summary>
    enum XEncodeType
    {
        /// <summary>
        /// 源数据不重新编码
        /// </summary>
        XNone,
        /// <summary>
        /// 源数据重新GZip压缩编码
        /// </summary>
        XGZip,
    };

    //Add By Y-way
    //The header of encryption file.
    typedef struct
    {
        uint32_t sign;// Signature code.
        uint8_t offset;// Rawdata offset form the file begin.
        uint8_t encrypt_data_size; // Encrypt the length of data.
        uint8_t encode_type; // The data encode type.0:default,do nothing. 1: zip compress.
        uint8_t version; // The encode version
        uint32_t length; // The rawdata length.
        uint32_t reserved; //Reserved for users. 
    }XEFHeader;
}
