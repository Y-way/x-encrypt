namespace XEncryptAPI
{
    /// <summary>
    /// 加密/解密结果状态码
    /// </summary>
    public enum ResultCode : int
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
        /// 无效的加密/解密插件
        /// </summary>
        InvalidPlugin,
        /// <summary>
        /// 无效的输入数据
        /// </summary>
        InvalidInputData,
        /// <summary>
        /// 无效的上下文
        /// </summary>
        InvalidXContext,
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
