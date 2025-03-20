#pragma once

#include "config.h"
#include "Common.h"
#include "XContext.h"

namespace xencrypt
{
    class XENCRYPT_API XResult final
    {
    public:
        XResult():_context(nullptr) {}
        ~XResult();

        XResult(const XResult&) = delete;
        XResult(XResult&&) = delete;
        XResult& operator=(const XResult&) = delete;
        
        byte* GetData() const
        {
            return _context != nullptr ? _context->GetResultData() : nullptr;
        }

        int64_t GetDataSize() const
        {
            return _context != nullptr ? _context->GetResultDataLength() : 0;
        }

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