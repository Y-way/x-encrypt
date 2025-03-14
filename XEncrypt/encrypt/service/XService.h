#pragma once

#include "plugin/XPlugin.h"
#include "XResult.h"

namespace xencrypt
{
	/// @brief 加密/解密服务类
	class XENCRYPT_API XService
	{
    public:
        XService(XPlugin* plugin) :_plugin(plugin) {}
        ~XService() = default;

        XService(const XService& other) = delete;
        XService(XService&& other) = delete;
        XService& operator=(const XService&) = delete;

        /// @brief 检查数据是否已加密
        /// @param data 内存数据地址
        /// @param size 数据长度
        /// @return 数据是否已加密.返回true,数据已加密,否则,未加密.
        bool IsEncrypted(const byte* data, int64_t size);

        /// @brief 数据解密处理
		/// @param in 已加密数据
		/// @param length 输入数据长度
		/// @param cloneInput 是否复制内存数据
		/// @return 返回解密结果指针.该指针需要手动释放.
        XResult* Decrypt(const byte* in, int64_t length, bool cloneInput = false);
		
        /// @brief 数据加密
		/// @param in 待加密数据
		/// @param length 数据长度
        /// @return 返回加密结果指针.该指针需要手动释放.
        XResult* Encrypt(const byte* in, int64_t length);
	private:
		XPlugin* _plugin;
	};
}
