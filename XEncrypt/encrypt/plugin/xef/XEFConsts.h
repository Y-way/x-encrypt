#pragma once

namespace encrypt
{    /// <summary>
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