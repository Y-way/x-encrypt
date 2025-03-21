using System;
using System.Runtime.InteropServices;
using static XEncryptAPI.XService;

namespace XEncryptAPI
{
    /// <summary>
    /// 加密Scope
    /// </summary>
    public class EncryptScope : IDisposable
    {
        IntPtr _plugin = IntPtr.Zero;
        IntPtr _service = IntPtr.Zero;
        public EncryptScope(XEFPlugin.XEncodeType type, byte encryptSize = 16)
        {
#if DEBUG
            DebugLog("EncryptScope Initialize");
#endif
            _plugin = XEFPlugin.Create(type, encryptSize);
        }

        public void Begin()
        {
#if DEBUG
            DebugLog("EncryptScope Begin");
#endif
            _service = XService.Initialize(_plugin);
        }

        public ResultCode EncryptData(byte[] rawdata, out byte[] data, byte encryptSize = 16, XEFPlugin.XEncodeType type = XEFPlugin.XEncodeType.XNone)
        {
#if DEBUG
            DebugLog("EncryptScope EncryptData");
#endif
            data = rawdata;
            unsafe
            {
                fixed(byte* rawdataPtr = rawdata)
                {
                    if(XService.IsEncrypted(_service, rawdataPtr, rawdata.LongLength))
                    {
                        return ResultCode.EncryptedData;
                    }

                    XResult result = XService.Encrypt(_service, rawdataPtr, rawdata.LongLength);
#if DEBUG
                    DebugLog($"Encrypt data state({result.code})");
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
            DebugLog("EncryptScope End");
#endif
            if(_service != IntPtr.Zero)
            {
                XService.Deinitialize(_service);
            }
            _service = IntPtr.Zero;
        }

        public void Dispose()
        {
#if DEBUG
            DebugLog("EncryptScope UnInitialize");
#endif
            if(_plugin != IntPtr.Zero)
            {
                XEFPlugin.Destroy(_plugin);
            }
            _plugin = IntPtr.Zero;
        }
    }

}
