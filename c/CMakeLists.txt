#
# Copyright (c) 2022- ThunderSoft
# All Rights Reserved by Thunder Software Technology Co., Ltd and its affiliates.
# You may not use, copy, distribute, modify, transmit in any form this file
# except in compliance with ThunderSoft in writing by applicable law.
#

#
# @file    CMakeLists
# @brief   brief function description.
# @details detailed function description.
# @version 1.0
# @author  LiuZiRui
# @date    2022-04-29
#
# Edit History
# ----------------------------------------------------------------------------
# DATE                     NAME               DESCRIPTION
# 2022-04-29               LiuZiRui           Create it.
#
set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required(VERSION 3.4)

set(PROJECT_PATH ${PROJECT_PATH} "/home/ts/code/d11")

SET(CMAKE_TOOLCHAIN_FILE /home/ts/android-ndk-r25c/build/cmake/android.toolchain.cmake)
#set (CMAKE_LIBRARY_PATH ),环境变量的path
#set (CMAKE_INCLUDE_PATH )
#SET(CMAKE_C_STANDARD 
SET(CMAKE_CXX_STANDARD c++17)
#SET(CMAKE_FIND_ROOT_PATH
SET(CMAKE_SYSROOT /home/ts/android-ndk-r25c/toolchains/llvm/prebuilt/linux-x86_64/sysroot)
SET(CMAKE_C_COMPILER /home/ts/android-ndk-r25c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang)
SET(CMAKE_CXX_COMPILER /home/ts/android-ndk-r25c/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -DPCCTS_USE_NAMESPACE_STD \
	-Wno-unused-variable -Wno-unused-parameter -Wno-missing-field-initializers --target=aarch64-linux-android31 ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -DPCCTS_USE_NAMESPACE_STD \
	-Wno-unused-variable -Wno-unused-parameter -Wno-missing-field-initializers --target=aarch64-linux-android31 ")

project(carcam_feature_avm VERSION 0.1)

MESSAGE( STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
MESSAGE( STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
MESSAGE( STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

# add MACRO define
add_definitions("-D_ANDROID_")
add_definitions("-DCARCAM_ANDROID")
add_definitions("-DLUCKY_E04")
# add include path
include_directories(${PROJECT_PATH}/carcam/videocat/vc_os_inf/)
include_directories(${PROJECT_PATH}/carcam/videocat/vc_inf/)
include_directories(${PROJECT_PATH}/carcam/videocat/vc_carcam_inf/)
include_directories(${CMAKE_SOURCE_DIR}/../../inc/)
include_directories(${CMAKE_SOURCE_DIR}/../../src/)
include_directories(${PROJECT_PATH}/android_out/include/soong/ndk/sysroot/usr/include/)
# add libs path
link_directories(${PROJECT_PATH}/android_out/lib64_so/target/product/se1000_dx11_slave/system/lib64/)
message(stauts pzpz "${PROJECT_PATH}/android_out/lib64_so/target/product/se1000_dx11_slave/system/lib64/")
link_directories(${PROJECT_PATH}/carcam/thirdparty/lib64/androidi/)
message(stauts pzpz "${PROJECT_PATH}/carcam/thirdparty/lib64/android/")

#add_compile_options(-std=c++11)
# add source file
set(SRC_LIST ../../src/avm_manager.c
        ../../src/videocat_adapter.c)

# add target
add_library(carcam_feature_avm SHARED ${SRC_LIST})
# add shared lib
target_link_libraries(carcam_feature_avm
                carcam_ui_avm
                videocat
                )
#target_link_libraries(carcam ${LIBS})

install (TARGETS carcam_feature_avm DESTINATION bin)


