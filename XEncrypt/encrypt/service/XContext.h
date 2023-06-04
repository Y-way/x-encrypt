#pragma once

#include "config.h"
#include "Common.h"

namespace xencrypt
{

    class XENCRYPT_API XContext
    {
    public:
        XContext(XContextType type);
        ~XContext();
        
        XContextType GetType() const { return _type; }

        void SetCloneInputDataFlag(bool keep) { _clone = keep; }
        
        bool IsCloneInputData() const { return _clone; }

        void SetMemoryType(XCodeMemoryType type) { _memoryType = type;}
        XCodeMemoryType GetMemoryType() const { return _memoryType;}

        void SetResultCode(ResultCode code) { _code = code;}
        ResultCode GetResultCode() const { return _code;}

        void Reset();

        void ReleaseData();

        void SetInputData(const byte* data, int64_t length)
        {
            _input = data;
            _inputLength = length;
        }

        const byte* GetInputData() const { return _input;}
        int64_t GetInputDataLength() const {return _inputLength;}

        void SetResultData(byte* data, int64_t length)
        {
            this->_data = data;
            this->_length = length;
        }
        
        const byte* GetResultData() const {return _data;}
        
        byte* GetResultData() {return _data;}

        int64_t GetResultDataLength() const {return _length;}
    private:
        XContextType _type;
        ResultCode _code;
        XCodeMemoryType _memoryType;
        byte* _data;
        int64_t _length;
        const byte* _input;
        int64_t _inputLength;
        bool _clone;
    };

}
