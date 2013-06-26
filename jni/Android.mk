# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

#LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)
#LOCAL_MODULE    := lua
#LOCAL_CFLAGS    := -DLUA_USE_LINUX
#LOCAL_SRC_FILES := lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c lmathlib.c loslib.c lstrlib.c ltablib.c loadlib.c linit.c
#include $(BUILD_STATIC_LIBRARY)


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#LOCAL_ARM_MODE := arm
LOCAL_CFLAGS    := -DLUA_USE_LINUX -O3
LOCAL_MODULE    := hello-jni
#LOCAL_SRC_FILES := hello-jni.c
LOCAL_SRC_FILES := hello-jni.c
LOCAL_SRC_FILES := hello-jni.c lcallbenchmark.c lapi.c lauxlib.c lbaselib.c lcode.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c ltm.c lundump.c lvm.c lzio.c 
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -landroid

include $(BUILD_SHARED_LIBRARY)
