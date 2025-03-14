#include "XResult.h"

namespace xencrypt
{
    XResult::~XResult()
    {
        if (_context != nullptr)
        {
            delete _context;
        }
        _context = nullptr;

        _resultCode = ResultCode::Unknown;
    }
}
