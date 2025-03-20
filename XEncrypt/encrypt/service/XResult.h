#pragma once

#include "config.h"
#include "Common.h"
#include "XContext.h"

namespace xencrypt
{
    /// @brief 加密/解密结果
    class XENCRYPT_API XResult final
    {
    public:
        XResult():_context(nullptr) {}
        ~XResult();

        XResult(const XResult&) = delete;
        XResult(XResult&&) = delete;
        XResult& operator=(const XResult&) = delete;
        /// @brief 获取结果数据缓存地址指针.该数据在XResult对象销毁时会被释放.
        /// @return 结果数据缓存地址
        byte* GetData() const
        {
            return _context != nullptr ? _context->GetResultData() : nullptr;
        }
        /// @brief 获取结果数据大小
        int64_t GetDataSize() const
        {
            return _context != nullptr ? _context->GetResultDataLength() : 0;
        }
        /// @brief 加密/解密结果状态
        ResultCode GetCode() const { return _resultCode; }
    private:

        void SetResultCode(ResultCode code) { _resultCode = code; }

        void SetContext(XContext* context) { _context = context; }
    private:
        XContext* _context;
        ResultCode _resultCode = ResultCode::Unknown;
        friend class XService;
    };
}