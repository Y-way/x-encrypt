#include "common/Allocator.h"
#include "common/Logging.h"
#include "XService.h"
#include "Common.h"

namespace xencrypt
{
    static XService* s_instance = nullptr;

    XService::~XService()
    {
    }

    void XService::Initialize(XEncryptPlugin* plugin)
    {
        if (s_instance == nullptr)
        {
            s_instance = new XService(plugin);
        }
    }

    XEncryptPlugin* XService::RegisterPlugin(XEncryptPlugin* plugin)
    {
        if (s_instance == nullptr)
        {
            Logging::Write("The service instance dose not create.");
            return nullptr;
        }
        XEncryptPlugin* oldPlugin = s_instance->_plugin;
        s_instance->_plugin = plugin;
        return oldPlugin;
    }

    void XService::UnInitialize()
    {
        if (s_instance != nullptr)
        {
            delete s_instance;
            s_instance = nullptr;
        }
    }

    ResultCode XService::Decrypt(XContext* context, const byte* input, int64_t length, bool cloneInput /*= false*/)
    {
        if (s_instance == nullptr)
        {
            return ResultCode::UnInitialize;
        }
        if (context == nullptr)
        {
            return ResultCode::InvalidXContext;
        }
        if (s_instance->_plugin == nullptr)
        {
            return ResultCode::InvalidPlugin;
        }
        if (!s_instance->_plugin->IsSupport(context->GetType()))
        {
            context->SetResultCode(ResultCode::InvalidDecoder);
        }
        else
        {
            context->SetCloneInputDataFlag(cloneInput);
            context->SetInputData(input, length);
            s_instance->_plugin->Dencrypt(context);
        }
        return context->GetResultCode();
    }

    ResultCode XService::Encrypt(XContext* context, const byte* in, int64_t length)
    {
        if (s_instance == nullptr)
        {
            return ResultCode::UnInitialize;
        }
        if (context == nullptr)
        {
            return ResultCode::InvalidXContext;
        }
        if (s_instance->_plugin == nullptr)
        {
            return ResultCode::InvalidPlugin;
        }
        if (!s_instance->_plugin->IsSupport(context->GetType()))
        {
            context->SetResultCode(ResultCode::InvalidEncoder);
        }
        else
        {
            context->SetInputData(in, length);
            s_instance->_plugin->Encrypt(context);
        }
        return context->GetResultCode();
    }

    XContext* XService::CreateContext(XContextType type)
    {
        XContext* result = new XContext(type);
        if (result == nullptr)
        {
            Logging::Write("Out memory.");
            return nullptr;
        }
        return result;
    }

    void XService::ReleaseContext(XContext* context)
    {
        if (context == nullptr)
        {
            return;
        }
        delete context;
        context = nullptr;
    }
}
