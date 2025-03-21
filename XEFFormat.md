# XEFPlugin

自定加密/解密插件

### 数据头部(16字节):

|sign|offset|encrypt_data_size|encode_type|version|length|reserved|
|----|------|-----------------|-----------|-------|------|--------|
|<-  |      |                 |    16<br/>单位:byte     |       |      |      ->|
|  4 |   1  |      1          |    1      |   1   |   4  |    4   |
|@XFE|源数据偏移|加密数据长度|数据加密编码类型.<br/>0:默认,不GZip压缩.<br/> 1: GZip 压缩.|编码格式版本|源数据长度|保留字段|

### 数据头部数据结构定义:
``` C
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
    /// @brief 保留字段
    uint32_t reserved;
}XEFHeader;
/// @brief XEF格式数据加密头部标记
static const byte kXFileSignatureCode[] = { '@', 'X', 'F', 'E' };

```

## 加密数据格式:
|Header|EncryptedData|
|---------------|----------------------|
|数据头部  |加密数据|
|16字节长度|GZip压缩:<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;加密(GZip(源数据), 加密长度)<br/>非GZip压缩:<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;加密(源数据, 加密长度)|


## 加密算法:
参见`XEF加密算法`[byte* XEFEncoder::EncryptData()](./XEncrypt/encrypt/plugin/xef/XEFEncoder.cpp#L134)
```C
/// @brief 加密数据
/// @param rawdata 源数据.若支持GZip, rawdata=GZip(源数据), 否则为源数据.
/// @param size 加密数据长度.Min(Clamp(size, 1, 255), rawdata_size)
byte* EncryptData(byte* rawdata, int size)
{
#ifdef XEF_ENCRYPT_SERVICE
    //encrypt the data.
    for (int i = 0; i < size; i++)
    {
        byte* c = &rawdata[i];
        *c = ((*c << 0x04) | (*c >> 0x04));
    }
#endif // XEF_ENCRYPT_SERVICE
    return rawdata;
}
```
## 解密算法:

加密算法的逆过程.
参见`XEF解密算法`[bool XEFDecoder::DecryptData()](./XEncrypt/encrypt/plugin/xef/XEFDecoder.cpp#L45)
