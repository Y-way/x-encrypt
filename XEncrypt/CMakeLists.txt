# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#

#define_source_files（）

#Project(CmakeTest)
#aux_source_directory(. src)
#add_executable(project1 ${src})                        #编译为可执行程序
#add_library(project1 ${src})                           #编译为静态库
#add_library(project1 SHARED ${src})                    #编译为动态链接库

#add_executable(project1 MACOSX_BUNDLE ${src})          #编译为可执行程序 *.app

#add_library(project1 MODULE ${src})                    #编译为程序资源包 *.bundle
#set_target_properties(project1 PROPERTIES BUNDLE TRUE)

#add_library(project1 SHARED ${src})                     #编译为程序资源包 *.framework
#set_target_properties(project1 PROPERTIES FRAMEWORK TRUE)

#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")  # Debug模式下的编译指令
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")         # Release模式下的编译指令

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)       #设置可执行文件的输出目录

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)           #设置库文件的输出目录


#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)   

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin) 

#上面两条语句分别设置了Debug版本和Release版本可执行文件的输出目录,

#一旦设置上面的属性,在任何环境下生成的可执行文件都将直接放在你所设置的目录.

#四. set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../lib)    
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../lib) 

#上面两条语句分别设置了Debug版本和Release版本库文件的输出目录,

#一旦设置上面的属性,在任何环境下生成的库文件都将直接放在你所设置的目录.

# 当前目录:
# PROJECT_SOURCE_DIR：工程的根目录
# PROJECT_BINARY_DIR：运行cmake命令的目录，通常为${PROJECT_SOURCE_DIR}/build
# PROJECT_NAME：返回通过 project 命令定义的项目名称
# CMAKE_CURRENT_SOURCE_DIR：当前处理的 CMakeLists.txt 所在的路径
# CMAKE_CURRENT_BINARY_DIR：target 编译目录
# CMAKE_CURRENT_LIST_DIR：CMakeLists.txt 的完整路径
# EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置
# LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置

cmake_minimum_required(VERSION 3.13.0)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0048 NEW)

project ("CXEncrypt")

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

option(SUPPORT_ENCRYPT "Support encrypt data sevice" ON)
option(SUPPORT_DECRYPT "Support decrypt data sevice" ON)

option(LIB_SUPPORT_ENCRYPT "lib supports encrypt data sevice" ON)
option(LIB_SUPPORT_DECRYPT "lib supports decrypt data sevice" ON)

if(LIB_SUPPORT_ENCRYPT)
    set(SUPPORT_ENCRYPT ON)
endif()

if(LIB_SUPPORT_DECRYPT)
    set(SUPPORT_DECRYPT ON)
endif()

message(STATUS "PROJECT_SOURCE_DIR:${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR:${PROJECT_BINARY_DIR}")
message(STATUS "PROJECT_NAME:${PROJECT_NAME}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_LIST_DIR:${CMAKE_CURRENT_LIST_DIR}")

message(STATUS "SUPPORT_ENCRYPT:${SUPPORT_ENCRYPT}")
message(STATUS "SUPPORT_DECRYPT:${SUPPORT_DECRYPT}")

# 检测平台
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "Platform: Windows")
    set(PLATFORM_WINDOWS TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Platform: Linux")
    set(PLATFORM_LINUX TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "Platform: macOS")
    set(PLATFORM_MACOS TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
    message(STATUS "Platform: Android")
    set(PLATFORM_ANDROID TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "iOS")
    message(STATUS "Platform: iOS")
    set(PLATFORM_IOS TRUE)
endif()

# 设置平台特定的编译选项
if(PLATFORM_WINDOWS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")
    endif()
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
elseif(PLATFORM_LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    add_definitions(-DZ_HAVE_UNISTD_H)
elseif(PLATFORM_MACOS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    add_definitions(-DZ_HAVE_UNISTD_H)
elseif(PLATFORM_ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    add_definitions(-DZ_HAVE_UNISTD_H)
elseif(PLATFORM_IOS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    add_definitions(-DZ_HAVE_UNISTD_H)
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

# 在 iOS 平台下不生成 SHARED 库和 CXEncrypt 可执行文件
if(NOT PLATFORM_IOS)
    
    add_executable (CXEncrypt CXEncrypt.cpp CXEncrypt.h)
    target_link_libraries(CXEncrypt libXEncrypt)

    set_target_properties(CXEncrypt
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
    )

    if (CMAKE_VERSION VERSION_GREATER 3.12)
        set_property(TARGET CXEncrypt PROPERTY CXX_STANDARD 14)
    endif()
endif()

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

# 设置输出目录
if(PLATFORM_WINDOWS)
    set(OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
elseif(PLATFORM_LINUX OR PLATFORM_MACOS OR PLATFORM_ANDROID OR PLATFORM_IOS)
    set(OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_PATH})

message(STATUS "EXECUTABLE_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}")
