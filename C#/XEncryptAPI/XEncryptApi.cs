using System;
using System.Runtime.InteropServices;
namespace XEncryptAPI
{
    public sealed class XService : IDisposable
    {
        /// <summary>
        /// 加密/解密结果
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct XResult
        {
            /// <summary>
            /// 结果状态码
            /// </summary>
            public ResultCode code;
            /// <summary>
            /// 结果数据大小
            /// </summary>
            public long size;
            /// <summary>
            /// 结果数据
            /// </summary>
            public IntPtr data;
            /// <summary>
            /// XReult指针, 用于释放内存
            /// </summary>
            private IntPtr result;
        }

        #region C# API
        private IntPtr _service;

        public XService(IPlugin plugin)
        {
            _service = XService.Initialize(plugin.Native);
        }

        public unsafe bool IsEncrypted(byte[] data, long size)
        {
            fixed(byte* ptr = data)
            {
                return XService.IsEncrypted(_service, ptr, size);
            }
        }

        public unsafe XResult Encrypt(byte[] data, long size)
        {
            fixed(byte* ptr = data)
            {
                return XService.Encrypt(_service, ptr, data.Length);
            }
        }

        public unsafe XResult Decrypt(byte[] data, long size)
        {
            fixed(byte* ptr = data)
            {
                return XService.Decrypt(_service, ptr, data.Length);
            }
        }

        public unsafe void ReleaseResult(XResult result)
        {
            XService.ReleaseResult(_service, ref result);
        }

        public void Dispose()
        {
            if(_service != IntPtr.Zero)
            {
                Deinitialize(_service);
            }
            _service = IntPtr.Zero;
        }
        #endregion

        #region Log
        public static void DebugLog(string format, params object[] args)
        {
#if UNITY_EDITOR
            UnityEngine.Debug.LogFormat(format, args);
#else
            Console.WriteLine(format, args);
#endif
        }

        public static void DebugWarn(string format, params object[] args)
        {
#if UNITY_EDITOR
            UnityEngine.Debug.LogWarningFormat(format, args);
#else
            Console.WriteLine(format, args);
#endif
        }

        public static void DebugError(string format, params object[] args)
        {
#if UNITY_EDITOR
            UnityEngine.Debug.LogErrorFormat(format, args);
#else
            Console.WriteLine(format, args);
#endif
        }
        #endregion

        #region Native API
        /// <summary>
        /// 初始化服务 <br />
        /// void* xencrypt_service_initialize(void* plugin)
        /// </summary>
        /// <param name="plugin">加密/解密插件实例</param>
        /// <returns>加密/解密服务实例</returns>
        [DllImport(NativeLibrary.Name, EntryPoint = "xencrypt_service_initialize", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Initialize(IntPtr plugin);

        /// <summary>
        /// 检查数据是否已加密 <br />
        /// bool xencrypt_service_is_encrypted(void* service, const byte* data, int64_t size)
        /// </summary>
        /// <param name="service">加密/解密服务实例</param>
        /// <param name="data">内存数据地址</param>
        /// <param name="size">数据长度</param>
        /// <returns>数据是否已加密.返回true,数据已加密,否则,未加密.</returns>
        [DllImport(NativeLibrary.Name, EntryPoint = "xencrypt_service_is_encrypted", CallingConvention = CallingConvention.Cdecl)]
        public static extern unsafe bool IsEncrypted(IntPtr service, byte* data, long size);
        /// <summary>
        /// 加密数据 <br />
        /// void* xencrypt_service_encrypt(void* service, const byte* in, int64_t in_size)
        /// <param name="service">加密/解密服务实例</param>
        /// <param name="inData">待加密数据</param>
        /// <param name="size">待加密数据长度</param>
        /// <returns>解密结果实例指针</returns>
        [DllImport(NativeLibrary.Name, EntryPoint = "xencrypt_service_encrypt", CallingConvention = CallingConvention.Cdecl)]
        public static extern unsafe XResult Encrypt(IntPtr service, byte* inData, long size);
        /// <summary>
        /// 解密数据 <br />
        /// void* xencrypt_service_decrypt(void* service, const byte* in, int64_t in_size)
        /// </summary>
        /// <param name="service">加密/解密服务实例</param>
        /// <param name="inData">待解密数据</param>
        /// <param name="size">密数据长度</param>
        /// <returns>解密结果实例指针</returns>
        [DllImport(NativeLibrary.Name, EntryPoint = "xencrypt_service_decrypt", CallingConvention = CallingConvention.Cdecl)]
        public static extern unsafe XResult Decrypt(IntPtr service, byte* inData, long size, bool cloneInput = false);
        /// <summary>
        /// 销毁结果 <br />
        /// void xencrypt_service_release_result(void* service, void* result)
        /// </summary>
        /// <param name="service">加密/解密服务实例</param>
        /// <param name="result">加/解密结果指针</param>
        /// <returns></returns>
        [DllImport(NativeLibrary.Name, EntryPoint = "xencrypt_service_release_result", CallingConvention = CallingConvention.Cdecl)]
        public static extern unsafe void ReleaseResult(IntPtr service, ref XResult result);
        /// <summary>
        /// 注销服务 <br />
        /// void xencrypt_service_deinitialize(void* service)
        /// </summary>
        /// <param name="service">加密/解密服务实例</param>
        [DllImport(NativeLibrary.Name, EntryPoint = "xencrypt_service_deinitialize", CallingConvention = CallingConvention.Cdecl)]
        public static extern unsafe void Deinitialize(IntPtr service);
        #endregion
    }
}
