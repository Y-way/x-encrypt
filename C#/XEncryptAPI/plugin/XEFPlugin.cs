using System;
using System.Runtime.InteropServices;

namespace XEncryptAPI
{
    public sealed class XEFPlugin : IPlugin, IDisposable
    {
        #region C# API
        private IntPtr _native;
        /// <summary>
        /// 加密服务时,源数据加密编码格式
        /// </summary>
        public enum XEncodeType : int
        {
            /// <summary>
            /// 源文件不作处理,只增加加密文件头部
            /// </summary>
            XNone,
            /// <summary>
            /// 源数据重新GZip压缩编码
            /// </summary>
            XGZip,
        };

        public IntPtr Native => _native;

        public XEFPlugin(XEncodeType type, byte encryptSize = 16)
        {
            _native = Create(type, encryptSize);
        }

        public void Dispose()
        {
            if(_native != IntPtr.Zero)
            {
                Destroy(_native);
            }
            _native = IntPtr.Zero;
        }
        #endregion

        #region Native API

        /// <summary>
        /// 创建XEF格式加密/解密器插件实例 <br />
        /// void* xef_plugin_create(int type, uint8_t encryptSize)
        /// </summary>
        /// <param name="type">数据加密编码类型</param>
        /// <param name="encryptSize">数据加密长度</param>
        /// <returns>插件实例指针</returns>
        [DllImport(NativeLibrary.Name, EntryPoint = "xef_plugin_create", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Create(XEncodeType type, byte encryptSize);

        /// <summary>
        /// 销毁XEF格式 加密/解密插件实例 <br />
        /// void xef_plugin_destroy(void* plugin)
        /// </summary>
        /// <param name="plugin">已创建的插件实例</param>
        [DllImport(NativeLibrary.Name, EntryPoint = "xef_plugin_destroy", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Destroy(IntPtr plugin);
    }
    #endregion
}
