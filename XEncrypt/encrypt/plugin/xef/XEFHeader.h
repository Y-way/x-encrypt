#pragma once
namespace xencrypt
{
    /// @brief XEF格式数据加密头部
    typedef struct
    {
        /// @brief 格式签名标记
        uint32_t sign;
        /// @brief 源数据偏移
        uint8_t offset;
        /// @brief 加密数据长度
        uint8_t encrypt_data_size;
        /// @brief 数据加密编码类型.0:默认,不做处理. 1: gzip 压缩.
        uint8_t encode_type;
        /// @brief 编码格式版本
        uint8_t version;
        /// @brief 源数据长度
        uint32_t length;
        /// @brief 预留字段
        uint32_t reserved;
    }XEFHeader;

    /// @brief XEF格式数据加密头部标记
    static const byte kXFileSignatureCode[] = { '@', 'X', 'F', 'E' };
    template<typename T = uint32_t>
    bool IS_XFILE_SIGNATURE_CODE(T code) { return (code == *((uint32_t*)(&kXFileSignatureCode))); }
}
