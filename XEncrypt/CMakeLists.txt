cmake_minimum_required (VERSION 3.8)

# 当前目录:
# PROJECT_SOURCE_DIR：工程的根目录
# PROJECT_BINARY_DIR：运行cmake命令的目录，通常为${PROJECT_SOURCE_DIR}/build
# PROJECT_NAME：返回通过 project 命令定义的项目名称
# CMAKE_CURRENT_SOURCE_DIR：当前处理的 CMakeLists.txt 所在的路径
# CMAKE_CURRENT_BINARY_DIR：target 编译目录
# CMAKE_CURRENT_LIST_DIR：CMakeLists.txt 的完整路径
# EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置
# LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置

option(SUPPORT_ENCRYPT "Support encrypt data sevice" ON)
option(SUPPORT_DECRYPT "Support decrypt data sevice" ON)

option(LIB_SUPPORT_ENCRYPT "lib supports encrypt data sevice" ON)
option(LIB_SUPPORT_DECRYPT "lib supports decrypt data sevice" ON)

message(STATUS "PROJECT_SOURCE_DIR:${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR:${PROJECT_BINARY_DIR}")
message(STATUS "PROJECT_NAME:${PROJECT_NAME}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_LIST_DIR:${CMAKE_CURRENT_LIST_DIR}")

message(STATUS "SUPPORT_ENCRYPT:${SUPPORT_ENCRYPT}")
message(STATUS "SUPPORT_DECRYPT:${SUPPORT_DECRYPT}")

# 设置输出目录
set(OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/x86_64")

# 设置可执行文件的输出路径：
# set(EXECUTABLE_OUTPUT_PATH "${OUTPUT_PATH}/bin/")
# 库lib输出路径：
# set(LIBRARY_OUTPUT_PATH "${OUTPUT_PATH}/lib/")
# 拷贝文件到exe所在的目录
# 1 拷贝文件夹里的文件
# file(COPY ${OPENCV_DLL_DIR}/ DESTINATION ${EXECUTABLE_OUTPUT_PATH})
# 2 拷贝bin文件
# file(COPY ${OPENCV_DLL_DIR} DESTINATION ${EXECUTABLE_OUTPUT_PATH})

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# CMake设置输出目录
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})

# 以上设置VS会自动新建Debug和Release文件夹
# 以下设置分别设置Debug和Release输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})

message(STATUS "EXECUTABLE_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}")

if(MSVC)
    #上面两条语句分别设置了Debug版本和Release版本下库文件的后缀名
    #set(CMAKE_DEBUG_POSTFIX "_d")    set(CMAKE_RELEASE_POSTFIX "_r") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

    if (CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    endif()
    set(CMAKE_DEBUG_POSTFIX "d")
    # 定义宏
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

# 添加 include 路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/encrypt")

set(XXHASH_HDRS
    encrypt/xxHash/xxh3.h
    encrypt/xxHash/xxhash.h
)

set(XXHASH_SRCS
    encrypt/xxHash/xxhash.c
)

set(ZLIB_PUBLIC_HDRS
    encrypt/zlib/zconf.h
    encrypt/zlib/zlib.h
)

set(ZLIB_PRIVATE_HDRS
    encrypt/zlib/crc32.h
    encrypt/zlib/deflate.h
    encrypt/zlib/gzguts.h
    encrypt/zlib/inffast.h
    encrypt/zlib/inffixed.h
    encrypt/zlib/inflate.h
    encrypt/zlib/inftrees.h
    encrypt/zlib/trees.h
    encrypt/zlib/zutil.h
)

set(ZLIB_SRCS
    encrypt/zlib/adler32.c
    encrypt/zlib/compress.c
    encrypt/zlib/crc32.c
    encrypt/zlib/deflate.c
    encrypt/zlib/gzclose.c
    encrypt/zlib/gzlib.c
    encrypt/zlib/gzread.c
    encrypt/zlib/gzwrite.c
    encrypt/zlib/inflate.c
    encrypt/zlib/infback.c
    encrypt/zlib/inftrees.c
    encrypt/zlib/inffast.c
    encrypt/zlib/trees.c
    encrypt/zlib/uncompr.c
    encrypt/zlib/zutil.c
)

set(XENCRYPT_PUBLIC_HDRS
    encrypt/api_types.h
    encrypt/config.h
    encrypt/config_api.h
    encrypt/config_api_platforms.h
    encrypt/config_platforms.h
    encrypt/RuntimeApi.h)

set(XENCRYPT_PRIVATE_HDRS
    encrypt/common/Allocator.h
    encrypt/common/XAssert.h
    encrypt/common/Logging.h
    encrypt/common/ZipUtils.h 
    encrypt/service/Common.h 
    encrypt/service/XContext.h
    encrypt/service/XService.h
    encrypt/plugin/Decoder.h
    encrypt/plugin/Encoder.h
    encrypt/plugin/XEncryptPlugin.h)

set(XENCRYPT_PRIVATE_SRCS
    encrypt/common/Allocator.cpp
    encrypt/common/XAssert.cpp
    encrypt/common/Logging.cpp
    encrypt/common/ZipUtils.cpp
    encrypt/service/XService.cpp
    encrypt/service/XContext.cpp
    encrypt/api_types.h
    encrypt/config.h
    encrypt/config_api.h
    encrypt/config_api_platforms.h
    encrypt/config_platforms.h
    encrypt/RuntimeApi.cpp)

set(XENCRYPT_PLGUIN_XEF_HDRS
    encrypt/plugin/xef/XEFHeader.h
    encrypt/plugin/xef/XEncodeType.h
    encrypt/plugin/xef/XEFDecoder.h
    encrypt/plugin/xef/XEFEncoder.h
    encrypt/plugin/xef/XEFEncryptPlugin.h
    encrypt/plugin/xef/XEFRuntimeApi.h
)

set(XENCRYPT_PLGUIN_XEF_SRCS
    encrypt/plugin/xef/XEFDecoder.cpp
    encrypt/plugin/xef/XEFEncoder.cpp
    encrypt/plugin/xef/XEFEncryptPlugin.cpp
    encrypt/plugin/xef/XEFRuntimeApi.cpp
)

add_library(XEncrypt SHARED 
    ${XXHASH_SRCS} 
    ${XXHASH_HDRS} 
    ${ZLIB_SRCS} ${ZLIB_DLL_SRCS} 
    ${ZLIB_PUBLIC_HDRS} 
    ${ZLIB_PRIVATE_HDRS} 
    ${XENCRYPT_PUBLIC_HDRS} 
    ${XENCRYPT_PRIVATE_HDRS} 
    ${XENCRYPT_PRIVATE_SRCS}
    ${XENCRYPT_PLGUIN_XEF_HDRS}
    ${XENCRYPT_PLGUIN_XEF_SRCS})

add_library(libXEncrypt STATIC 
    ${XXHASH_SRCS} 
    ${XXHASH_HDRS}
    ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS}
    ${ZLIB_PRIVATE_HDRS}
    ${XENCRYPT_PUBLIC_HDRS}
    ${XENCRYPT_PRIVATE_HDRS} 
    ${XENCRYPT_PRIVATE_SRCS}
    ${XENCRYPT_PLGUIN_XEF_HDRS}
    ${XENCRYPT_PLGUIN_XEF_SRCS})

set_target_properties(XEncrypt PROPERTIES DEFINE_SYMBOL XENCRYPT_EXPORT_API)

if(SUPPORT_ENCRYPT)
    target_compile_definitions(XEncrypt PUBLIC XEF_ENCRYPT_SERVICE)
    if(LIB_SUPPORT_ENCRYPT)
        target_compile_definitions(libXEncrypt PUBLIC XEF_ENCRYPT_SERVICE)
    endif()
endif()

if(SUPPORT_DECRYPT)
    target_compile_definitions(XEncrypt PUBLIC XEF_DECRYPT_SERVICE)
    if(LIB_SUPPORT_DECRYPT)
        target_compile_definitions(libXEncrypt PUBLIC XEF_DECRYPT_SERVICE)
    endif()
endif()

set_target_properties(XEncrypt PROPERTIES SOVERSION 1)

# 将源代码添加到此项目的可执行文件。
# 可执行文件名，通过指定的源文件列表构建出可执行目标文件：
add_executable (CXEncrypt CXEncrypt.cpp CXEncrypt.h)
# 设置链接库名称：
target_link_libraries(CXEncrypt libXEncrypt)

set_target_properties(CXEncrypt
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#设置了Debug版本和Release版本下可执行文件的后缀名
#set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")     set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "_r")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CXEncrypt PROPERTY CXX_STANDARD 14)
endif()