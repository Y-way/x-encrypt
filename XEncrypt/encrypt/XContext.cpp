#include "XContext.h"
#include "common/Allocator.h"
namespace encrypt
{
    XContext::XContext(XContextType type)
        :_type(type), _clone(false), _data(nullptr), _input(nullptr)
    {
        Reset();
    }

    XContext::~XContext()
    {
        { Reset(); }
    }

    void XContext::Reset()
    {
        ReleaseData();
        _clone = false;
        _code = ResultCode::Unknown;
        _memoryType = XCodeMemoryType::None;
        _input = nullptr;
        _inputLength = 0;
    }

    void XContext::ReleaseData()
    {
        _length = 0;
        if (_type == XCodeMemoryType::AllocateMemory && _data != nullptr)
        {
            XMEMORY_FREE(_data);
            _data = nullptr;
        }
    }
}