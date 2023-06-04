#pragma once

#include "config.h"
#include "Common.h"

namespace xencrypt
{
    /// @brief 加密/解密上下文
    class XENCRYPT_API XContext
    {
    public:
        XContext(XContextType type);
        ~XContext();
        /// @brief 获取上下文类型
        /// @return 上下文类型
        XContextType GetType() const { return _type; }
        /// @brief 设置是否复制输入数据
        /// @param keep 标记是否复制数据数据.true:深度复制;否则,不复制.
        void SetCloneInputDataFlag(bool keep) { _clone = keep; }
        /// @brief 是否复制输入数据
        /// @return 是否复制数据数据.true:深度复制;否则,不复制.
        bool IsCloneInputData() const { return _clone; }
        /// @brief 设置内存申请类型
        /// @param type 
        void SetMemoryType(XCodeMemoryType type) { _memoryType = type;}
        XCodeMemoryType GetMemoryType() const { return _memoryType;}
        /// @brief 设置加密/解密结果状态码
        /// @param code 状态码
        void SetResultCode(ResultCode code) { _code = code;}
        /// @brief 获取加密/解密结果状态码
        /// @return 加密/解密结果状态码
        ResultCode GetResultCode() const { return _code;}
        /// @brief 重置
        void Reset();
        /// @brief 释放申请的内存数据
        void ReleaseData();
        /// @brief 设置输入数据
        /// @param data 源数据
        /// @param length 源数据长度
        void SetInputData(const byte* data, int64_t length)
        {
            _input = data;
            _inputLength = length;
        }
        /// @brief 获取源数据
        /// @return 源数据地址
        const byte* GetInputData() const { return _input;}
        /// @brief 获取源数据长度
        /// @return 源数据长度
        int64_t GetInputDataLength() const {return _inputLength;}
        /// @brief 设置结果数据
        /// @param data 结果数据地址
        /// @param length 结果数据长度
        void SetResultData(byte* data, int64_t length)
        {
            this->_data = data;
            this->_length = length;
        }
        /// @brief 获取结果数据地址
        /// @return 结果数据地址
        const byte* GetResultData() const {return _data;}
        /// @brief 获取结果数据地址
        /// @return 结果数据地址
        byte* GetResultData() {return _data;}
        /// @brief 获取结果数据长度
        /// @return 结果数据长度
        int64_t GetResultDataLength() const {return _length;}
    private:
        XContextType _type;
        ResultCode _code;
        XCodeMemoryType _memoryType;
        byte* _data;
        int64_t _length;
        const byte* _input;
        int64_t _inputLength;
        bool _clone;
    };

}
