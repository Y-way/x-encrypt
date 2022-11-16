#include "XContext.h"
#include "Allocator.h"
namespace encrypt
{
    void XContext::Reset()
    {
        ReleaseData();
        _code = ResultCode::Unknown;
        _memoryType = XCodeMemoryType::None;
        _origin = nullptr;
        _originalLength = 0;
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