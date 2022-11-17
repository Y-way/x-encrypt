#pragma once

namespace encrypt
{
    enum XCodeMemoryType
    {
        None,
        AllocateMemory,
        OriginalOffset,
    };
    
    enum XContextType
    {
        XUnknown,
        Ecrypt,
        Decrypt,
    };

    enum ResultCode
    {
        Ok,
        Unknown,
        UnInitialize,
        InvalidInputData,
        InvalidInputDataSize,
        InvalidDecoder,
        InvalidUnzip,
        InvalidEncoder,
        InvalidZip,
        EncryptedData,
        OutMemory,
        ContextTypeError,
        NotSupportDecrypt,
        NotSupportEncrypt,
    };

    enum XEncodeType
    {
        XNone,
        XZip,
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
