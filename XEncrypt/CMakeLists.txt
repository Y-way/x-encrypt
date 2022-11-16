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

message(STATUS "PROJECT_SOURCE_DIR:${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_BINARY_DIR:${PROJECT_BINARY_DIR}")
message(STATUS "PROJECT_NAME:${PROJECT_NAME}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR:${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR:${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_LIST_DIR:${CMAKE_CURRENT_LIST_DIR}")

message(STATUS "SUPPORT_ENCRYPT:${SUPPORT_ENCRYPT}")
message(STATUS "SUPPORT_DECRYPT:${SUPPORT_DECRYPT}")
# 安装拷贝
#设置变量
# set(OPENCV_DLL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/OpenCV/bin)
# 设置输出目录
#set(OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(OUTPUT_PATH "${PROJECT_SOURCE_DIR}/../../Assets/Plugins/x86_64")
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


# install
# install用于指定在安装时运行的规则。它可以用来安装很多内容，可以包括目标二进制、动态库、静态库以及文件、目录、脚本等：
# install(TARGETS <target>... [...])
# install({FILES | PROGRAMS} <file>... [...])
# install(DIRECTORY <dir>... [...])
# install(SCRIPT <file> [...])
# install(CODE <code> [...])
# install(EXPORT <export-name> [...])
# 一般复制或文件夹
# install(DIRECTORY ${PROJECT_BINARY_DIR}/bin DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
# //将生成的文件夹下内容全部拷贝到，指定目标目录；

message(STATUS "EXECUTABLE_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}")

if(MSVC)
    #上面两条语句分别设置了Debug版本和Release版本下库文件的后缀名
    #set(CMAKE_DEBUG_POSTFIX "_d")    set(CMAKE_RELEASE_POSTFIX "_r") 
    set(CMAKE_DEBUG_POSTFIX "d")
    # 定义宏
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

# 添加 include 路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/encrypt")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/encrypt/common")

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
    "encrypt/api_types.h"
    "encrypt/config.h"
    "encrypt/config_api.h"
    "encrypt/config_api_platforms.h"
    "encrypt/config_platforms.h"
    "encrypt/RuntimeApi.h")

set(XENCRYPT_PRIVATE_HDRS
    "encrypt/common/Allocator.h"
    "encrypt/common/XAssert.h"
    "encrypt/common/Common.h" 
    "encrypt/common/Logging.h"
    "encrypt/common/ZipUtils.h" 
    "encrypt/Decoder.h"
    "encrypt/XService.h"
    "encrypt/XEFDecoder.h"
    "encrypt/XEFEncoder.h"
    "encrypt/XContext.h" 
    "encrypt/Encoder.h")

set(XENCRYPT_PRIVATE_SRCS
    "encrypt/common/Allocator.cpp"
    "encrypt/common/XAssert.cpp"
    "encrypt/common/Logging.cpp"
    "encrypt/common/ZipUtils.cpp"
    "encrypt/XService.cpp"
    "encrypt/api_types.h"
    "encrypt/config.h"
    "encrypt/config_api.h"
    "encrypt/config_api_platforms.h"
    "encrypt/config_platforms.h"
    "encrypt/XEFEncoder.cpp"
    "encrypt/XEFDecoder.cpp"
    "encrypt/XContext.cpp"
    "encrypt/RuntimeApi.cpp")

add_library(XEncrypt SHARED 
    ${XXHASH_SRCS} ${XXHASH_HDRS} 
    ${ZLIB_SRCS} ${ZLIB_DLL_SRCS} 
    ${ZLIB_PUBLIC_HDRS} 
    ${ZLIB_PRIVATE_HDRS} 
    ${XENCRYPT_PUBLIC_HDRS} 
    ${XENCRYPT_PRIVATE_HDRS} 
    ${XENCRYPT_PRIVATE_SRCS})

add_library(libXEncrypt STATIC 
    ${XXHASH_SRCS} ${XXHASH_HDRS}
    ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS}
    ${ZLIB_PRIVATE_HDRS}
    ${XENCRYPT_PUBLIC_HDRS}
    ${XENCRYPT_PRIVATE_HDRS} 
    ${XENCRYPT_PRIVATE_SRCS} )

set_target_properties(XEncrypt PROPERTIES DEFINE_SYMBOL XENCRYPT_EXPORT_API)

if(SUPPORT_ENCRYPT)
target_compile_definitions(XEncrypt PUBLIC XEF_ENCRYPT_SERVICE)
target_compile_definitions(libXEncrypt PUBLIC XEF_ENCRYPT_SERVICE)
endif()

if(SUPPORT_DECRYPT)
target_compile_definitions(XEncrypt PUBLIC XEF_DECRYPT_SERVICE)
target_compile_definitions(libXEncrypt PUBLIC XEF_DECRYPT_SERVICE)
endif()

set_target_properties(XEncrypt PROPERTIES SOVERSION 1)

# set_target_properties( targets...
#    PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#)

# 为生成的target配置安装目录
#install(TARGETS MyLib
#        EXPORT MyLibTargets 
#        LIBRARY DESTINATION lib  # 动态库安装路径(可选)
#        ARCHIVE DESTINATION lib  # 静态库安装路径(可选)
#        RUNTIME DESTINATION bin  # 可执行文件安装路径(可选)
#        PUBLIC_HEADER DESTINATION include  # 头文件安装路径(可选)
#        )
# LIBRARY, ARCHIVE, RUNTIME, PUBLIC_HEADER是可选的，可以根据需要进行选择。 
# DESTINATION后面的路径可以自行制定，根目录默认为CMAKE_INSTALL_PREFIX,可以试用set方法进行指定，
# 如果使用默认值的话，Unix系统的默认值为 /usr/local, 
# Windows的默认值为 c:/Program Files/${PROJECT_NAME}。
# 比如字linux系统下若LIBRARY的安装路径指定为lib,即为/usr/local/lib。
# 所以要安装mymath mymathapp我们可以这样写


#install (TARGETS XEncrypt DESTINATION bin)
#install (FILES ${XENCRYPT_PUBLIC_HDRS} DESTINATION include)

# 将源代码添加到此项目的可执行文件。
# 可执行文件名，通过指定的源文件列表构建出可执行目标文件：
add_executable (CXEncrypt "CXEncrypt.cpp" "CXEncrypt.h")
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