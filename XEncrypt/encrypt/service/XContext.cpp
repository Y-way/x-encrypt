#include "XContext.h"
#include "XService.h"
#include "common/Allocator.h"

namespace xencrypt
{
    XContext::XContext(XContextType type, XService* service)
        :_type(type), _clone(false), _data(nullptr), _input(nullptr), _service(service)
    {
        Reset();
    }

    XContext::~XContext()
    {
        { Reset(); }
    }

    bool XContext::IsEncrypted(const byte* data, int64_t size)
    {
        return _service != nullptr ? _service->IsEncrypted(data, size) : false;
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
        if (_memoryType == XCodeMemoryType::AllocateMemory && _data != nullptr)
        {
            XMEMORY_FREE(_data);
            _data = nullptr;
        }
    }
}