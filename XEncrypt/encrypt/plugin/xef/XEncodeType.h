#pragma once

namespace xencrypt
{
    /// @brief 加密服务时,源数据加密编码格式
    enum XEncodeType
    {
        /// @brief 源数据不重新编码
        XNone,
        /// @brief 源数据重新GZip压缩编码
        XGZip,
    };

}