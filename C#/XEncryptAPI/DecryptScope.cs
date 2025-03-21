using System;
using System.Runtime.InteropServices;
using static XEncryptAPI.XService;

namespace XEncryptAPI
{
    /// <summary>
    /// 解密Scope
    /// </summary>
    public class DecryptScope : IDisposable
    {
        IntPtr _plugin = IntPtr.Zero;
        IntPtr _service = IntPtr.Zero;
        public DecryptScope()
        {
#if DEBUG
            DebugLog("DecryptScope Initialize");
#endif
            _plugin = XEFPlugin.Create(XEFPlugin.XEncodeType.XNone, 0);
        }

        public void Begin()
        {
#if DEBUG
            DebugLog("DecryptScope Begin");
#endif
            _service = XService.Initialize(_plugin);
        }

        public ResultCode DecryptData(byte[] rawdata, out byte[] data)
        {
#if DEBUG
            DebugLog("DecryptScope DecryptData");
#endif
            data = rawdata;
            unsafe
            {
                fixed(byte* rawdataPtr = rawdata)
                {
                    if(!XService.IsEncrypted(_service, rawdataPtr, rawdata.LongLength))
                    {
                        return ResultCode.InvalidInputData;
                    }

                    XResult result = XService.Decrypt(_service, rawdataPtr, rawdata.LongLength);
#if DEBUG
                    DebugLog($"Decrypt data state({result.code})");
#endif
                    if(result.code == ResultCode.Ok)
                    {
                        data = new byte[result.size];
                        Marshal.Copy(result.data, data, 0, (int)result.size);
                    }
                    XService.ReleaseResult(_service, ref result);
                    return result.code;
                }
            }
        }

        public void End()
        {
#if DEBUG
            DebugLog("DecryptScope End");
#endif
            if(_service != IntPtr.Zero)
            {
                XService.Deinitialize(_service);
            }
        }

        public void Dispose()
        {
            if(_plugin != IntPtr.Zero)
            {
                XEFPlugin.Destroy(_plugin);
            }
            _plugin = IntPtr.Zero;
#if DEBUG
            DebugLog("DecryptScope UnInitialize");
#endif

        }
    }

}
