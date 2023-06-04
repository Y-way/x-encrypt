#pragma once

#include <stdint.h>
#include "Common.h"
#include "XContext.h"

#include "plugin/Decoder.h"
#include "plugin/Encoder.h"
#include "plugin/XEncryptPlugin.h"

namespace xencrypt
{
	/// @brief 加密/解密服务类
	class XENCRYPT_API XService
	{
	public:
		/// @brief 初始化服务
		/// @param plugin 加密/解密插件实例
		static void Initialize(XEncryptPlugin* plugin);
		/// @brief 重新设置加密/解密插件实例
		/// @param plugin 插件实例
		/// @return 旧的插件实例
		static XEncryptPlugin* RegisterPlugin(XEncryptPlugin* plugin);
		/// @brief 注销服务
		static void UnInitialize();
		/// @brief 使用解密上下文进行数据解密处理
		/// @param context 解密类型上下文
		/// @param in 已加密数据
		/// @param length 输入数据长度
		/// @param cloneInput 是否复制内存数据
		/// @return 返回解密状态码
		static ResultCode Decrypt(XContext* context, const byte* in, int64_t length, bool cloneInput = false);
		/// @brief 使用加密上下文进行数据加密
		/// @param context 加密类型上下文
		/// @param in 待加密数据
		/// @param length 数据长度
		/// @return 返回加密状态码
		static ResultCode Encrypt(XContext* context, const byte* in, int64_t length);
		/// @brief 创建指定类型的服务上下文
		/// @param type 上下文类型
		/// @return 指定类型的服务上下文实例
		static XContext* CreateContext(XContextType type);
		/// @brief 销毁服务创建的上下文,并销毁有上下文持有的内存缓存数据
		/// @param context 服务上下文实例
		static void ReleaseContext(XContext* context);

	public:
		~XService();

	private:
		XService(XEncryptPlugin* plugin) :_plugin(plugin) {}
		XService(const XService& other) :_plugin(other._plugin) {}
		XService& operator=(const XService&) = default;

	private:
		XEncryptPlugin* _plugin;
	};
}
