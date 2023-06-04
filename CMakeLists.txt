# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

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

project ("CXEncrypt")
set_property (GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory("XEncrypt")

