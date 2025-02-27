# A multi-platform support c++11 library with focus on asynchronous socket I/O for any client application.
#
# Copyright (c) 2012-2025 Y-Way.
#
# Licensed under the MIT License (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
# http://opensource.org/licenses/MIT
# Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
# See the License for the specific language governing permissions and limitations under the License.


cmake_minimum_required(VERSION 3.13.0)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0048 NEW)

set(xencrypt_target_name "XEncrypt")

project (${xencrypt_target_name})

set(_1kfetch_cache_dir "../${CMAKE_CURRENT_LIST_DIR}/cache" CACHE STRING "" FORCE)
set(_1kfetch_manifest "../${CMAKE_CURRENT_LIST_DIR}/manifest.json" CACHE STRING "" FORCE)

include(../1k/platforms.cmake)
include(../1k/fetch.cmake)

include(CMakeDependentOption)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

option(SUPPORT_ENCRYPT "Support encrypt data sevice" ON)
option(SUPPORT_DECRYPT "Support decrypt data sevice" ON)

set(XENCRYPT_ROOT ${CMAKE_CURRENT_LIST_DIR})

message(STATUS "XENCRYPT_ROOT=${XENCRYPT_ROOT}")
message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
message(STATUS "MSVC=${MSVC}, MSVC_IDE=${MSVC_IDE}")

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

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(FULL_MSVC TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(FULL_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(FULL_GCC TRUE)
endif()

# --- The C/C++ standard
if (NOT MSVC)
    if (NOT CXX_STD)
        set(CXX_STD 11)
    endif()
    set(CMAKE_CXX_STANDARD ${CXX_STD})
else()
    if (CXX_STD)
        set(CMAKE_CXX_STANDARD ${CXX_STD})
    else()
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("/std:c++latest" XENCRYPT_HAVE_CXX_LATEST)
        if (XENCRYPT_HAVE_CXX_LATEST)
            set(CXX_STD latest)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
        else()
            set(CXX_STD 11)
        endif()
    endif()
endif()

set (CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED ON)

message(STATUS "Building xencrypt with C++${CXX_STD} support")
message(STATUS "CMAKE_SYSTEM_NAME=" "${CMAKE_SYSTEM_NAME}")

function(xencrypt_config_target_outdir target)
    set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endfunction()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
    if(ANDROID)
        set(SYSTEM_STRING "Android")
    else()
        set(SYSTEM_STRING "Linux")
    endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
    set(UWP TRUE)
endif()

# The compiler flags
if(BUILD_SHARED_LIBS AND LINUX)
   set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -fPIC")
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

    if(FULL_GCC)
        set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
    else()
        add_definitions(-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE -D_SILENCE_ALL_CXX23_DEPRECATION_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
        remove_definitions(-D_MBCS)
    endif()

    if (FULL_MSVC)
        add_compile_options(/Bv /GF)
    else()
        add_definitions(-D__STDC_FORMAT_MACROS)
    endif()

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
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    add_definitions(-DZ_HAVE_UNISTD_H)
endif()

# 添加 include 路径
include_directories(${XENCRYPT_ROOT})
include_directories("${XENCRYPT_ROOT}/encrypt")

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

set(XENCRYPT_SOURCES
    ${XXHASH_SRCS} 
    ${XXHASH_HDRS}
    ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS}
    ${ZLIB_PRIVATE_HDRS}
    ${XENCRYPT_PUBLIC_HDRS}
    ${XENCRYPT_PRIVATE_HDRS} 
    ${XENCRYPT_PRIVATE_SRCS}
    ${XENCRYPT_PLGUIN_XEF_HDRS}
    ${XENCRYPT_PLGUIN_XEF_SRCS})

set(XENCRYPT_INC_DIRS
    ${XENCRYPT_ROOT}
    "${XENCRYPT_ROOT}/encrypt"
    )

# add_library(libXEncrypt STATIC 
#     ${XXHASH_SRCS} 
#     ${XXHASH_HDRS}
#     ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS}
#     ${ZLIB_PRIVATE_HDRS}
#     ${XENCRYPT_PUBLIC_HDRS}
#     ${XENCRYPT_PRIVATE_HDRS} 
#     ${XENCRYPT_PRIVATE_SRCS}
#     ${XENCRYPT_PLGUIN_XEF_HDRS}
#     ${XENCRYPT_PLGUIN_XEF_SRCS})

# add_library(XEncrypt SHARED 
#         ${XXHASH_SRCS} 
#         ${XXHASH_HDRS} 
#         ${ZLIB_SRCS} ${ZLIB_DLL_SRCS} 
#         ${ZLIB_PUBLIC_HDRS} 
#         ${ZLIB_PRIVATE_HDRS} 
#         ${XENCRYPT_PUBLIC_HDRS} 
#         ${XENCRYPT_PRIVATE_HDRS} 
#         ${XENCRYPT_PRIVATE_SRCS}
#         ${XENCRYPT_PLGUIN_XEF_HDRS}
#         ${XENCRYPT_PLGUIN_XEF_SRCS})

        
macro(source_group_by_dir proj_dir source_files)
if(MSVC OR APPLE)
    get_filename_component(sgbd_cur_dir ${proj_dir} ABSOLUTE)
    foreach(sgbd_file ${${source_files}})
        get_filename_component(sgbd_abs_file ${sgbd_file} ABSOLUTE)
        file(RELATIVE_PATH sgbd_fpath ${sgbd_cur_dir} ${sgbd_abs_file})
        string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        if(sgbd_nogroup)
            set(sgbd_group_name "\\")
        endif(sgbd_nogroup)
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach(sgbd_file)
endif(MSVC OR APPLE)
endmacro(source_group_by_dir)

source_group_by_dir(${CMAKE_CURRENT_SOURCE_DIR} XENCRYPT_SOURCES)


# set_target_properties(XEncrypt PROPERTIES SOVERSION 1)

# --- add yasio core lib project
if (APPLE)
    if (IOS)
        # set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
        add_library(${xencrypt_target_name} STATIC ${XENCRYPT_SOURCES})
    else ()
        # set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_64_BIT)")
        if (BUILD_SHARED_LIBS)
            add_library(${xencrypt_target_name} MODULE ${XENCRYPT_SOURCES})
            set_target_properties (${xencrypt_target_name} PROPERTIES BUNDLE TRUE )
        else()
            add_library(${xencrypt_target_name} STATIC ${XENCRYPT_SOURCES})
        endif()
    endif ()
elseif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Switch")
    add_library(${xencrypt_target_name} STATIC ${XENCRYPT_SOURCES})
    target_compile_options(${xencrypt_target_name} PRIVATE -m64 -mcpu=cortex-a57+fp+simd+crypto+crc -fno-common -fno-short-enums -ffunction-sections -fdata-sections -fPIC -fms-extensions)
else ( ) # linux/win32/android
    add_library(${xencrypt_target_name} ${XENCRYPT_SOURCES})
endif ( )

target_link_libraries(${xencrypt_target_name} ${XENCRYPT_EXTERN_LIBS})

message(STATUS "XENCRYPT_INC_DIRS=${XENCRYPT_INC_DIRS}")
target_include_directories(${xencrypt_target_name} PUBLIC ${XENCRYPT_INC_DIRS})

set(XENCRYPT_SERVICE "")

if(SUPPORT_ENCRYPT)
    set(XENCRYPT_SERVICE ${XENCRYPT_SERVICE} XEF_ENCRYPT_SERVICE)
endif()

if(SUPPORT_DECRYPT)
    set(XENCRYPT_SERVICE ${XENCRYPT_SERVICE} XEF_DECRYPT_SERVICE)
endif()

message(STATUS "XENCRYPT_SERVICE=${XENCRYPT_SERVICE}")

if (BUILD_SHARED_LIBS)
    target_compile_definitions(${xencrypt_target_name} PUBLIC  XENCRYPT_EXPORT_API ${XENCRYPT_SERVICE})
else()
    target_compile_definitions(${xencrypt_target_name} PUBLIC  XENCRYPT_IMPORT_API ${XENCRYPT_SERVICE})
endif()

macro(xencrypt_config_lib_depends target_name)
    target_link_libraries(${target_name} ${xencrypt_target_name})
    if ((NOT WIN32) AND (NOT ANDROID))
        target_link_libraries(${target_name} pthread)
    endif()
    
    # copy XEncrypt.dll
    if(BUILD_SHARED_LIBS AND WIN32)
        add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_BINARY_DIR}/\$\(Configuration\)/XEncrypt.dll"
                $<TARGET_FILE_DIR:${target_name}>
            )
    endif()
endmacro()

macro(xencrypt_config_app_folder target_name)
    set_target_properties(${target_name} PROPERTIES 
        FOLDER "apps")
endmacro()

macro(encrypt_config_app_depends target_name)
    xencrypt_config_lib_depends(${target_name})
    xencrypt_config_app_folder(${target_name})
endmacro(encrypt_config_app_depends)

# The yasio config preprocessors
macro(encrypt_config_pred target_name pred)
    if(${pred})
        target_compile_definitions(${target_name} PUBLIC ${pred}=1)
    endif()
endmacro()

macro(encrypt_config_option target_name optname optval)
    if(${optname})
        target_compile_definitions(${target_name} PUBLIC ${optname}=${optval})
    endif()
endmacro()

macro (encrypt_config_lib_options target_name)

    # if(SUPPORT_ENCRYPT)
    #     encrypt_config_pred(${target_name} XEF_ENCRYPT_SERVICE)
    # endif()
    # if(SUPPORT_DECRYPT)
    #     encrypt_config_pred(${target_name} XEF_DECRYPT_SERVICE)
    # endif()

    xencrypt_config_target_outdir(${xencrypt_target_name})
endmacro()

encrypt_config_lib_options(${xencrypt_target_name})

### Sets startup project for vs .sln
if (WIN32 AND XENCRYPT_BUILD_TESTS)
    add_executable (CXEncrypt CXEncrypt.cpp CXEncrypt.h)
    target_link_libraries(CXEncrypt libXEncrypt)

    set_target_properties(CXEncrypt
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PATH}"
    )

    if (CMAKE_VERSION VERSION_GREATER 3.12)
        set_property(TARGET CXEncrypt PROPERTY CXX_STANDARD 14)
    endif()    
endif ()
