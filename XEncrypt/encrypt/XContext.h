#pragma once

#include "config.h"
#include "Common.h"

namespace encrypt
{

    class XENCRYPT_API XContext
    {
    public:
        XContext(XContextType type):_type(type) { Reset(); };
        ~XContext() { Reset(); };
        
        XContextType GetType() const { return _type; }

        void SetMemoryType(XCodeMemoryType type) { _memoryType = type;}
        XCodeMemoryType GetMemoryType() const { return _memoryType;}

        void SetResultCode(ResultCode code) { _code = code;}
        ResultCode GetResultCode() const { return _code;}

        void Reset();

        void ReleaseData();

        void SetOriginalData(const byte* data, int64_t length)
        {
            _origin = data;
            _originalLength = length;
        }

        const byte* GetOriginalData() const { return _origin;}
        int64_t GetOriginalDataLength() const {return _originalLength;}

        void SetResultData(byte* data, int64_t length)
        {
            this->_data = data;
            this->_length = length;
        }
        
        //const byte* GetResultData() const {return _data;}
        
        byte* GetResultData() {return _data;}

        int64_t GetResultDataLength() const {return _length;}
    private:
        XContextType _type;
        ResultCode _code;
        XCodeMemoryType _memoryType;
        byte* _data;
        int64_t _length;
        const byte* _origin;
        int64_t _originalLength;
    };

}
