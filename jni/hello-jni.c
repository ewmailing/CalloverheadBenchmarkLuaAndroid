/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/asset_manager_jni.h> // for AAssetManager_fromJava
#include <android/asset_manager.h> // for other AAsetManager stuff
#include <android/log.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

struct lua_State* s_luaState = NULL;


extern int luaopen_callbenchmark(lua_State* lua_state);
static int report (lua_State *L, int status) {
	const char *msg;
	if (status) {
		msg = lua_tostring(L, -1);
		if (msg == NULL) msg = "(error with no message)";
		__android_log_print(ANDROID_LOG_INFO, "Hello-JNI", "status=%d, %s\n", status, msg);
		lua_pop(L, 1);
	}
	return status;
}
/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}

jboolean Java_com_example_hellojni_HelloJni_initLua(JNIEnv* env, jobject thiz, jobject java_asset_manager)
{

	struct lua_State* lua_state;
	int ret_flag;
	AAssetManager* asset_manager;
	AAsset* file_asset;
	off_t file_size;
	char* data_buffer;
	int bytes_read;
	jboolean is_good = JNI_TRUE;

	
    // Override point for customization after application launch.
	lua_state = luaL_newstate();
	s_luaState = lua_state;
	
	lua_pushcfunction(lua_state, luaopen_base);
	lua_pushliteral(lua_state, "");
	lua_call(lua_state, 1, 0);
	
	lua_pushcfunction(lua_state, luaopen_package);
	lua_pushliteral(lua_state, "package");
	lua_call(lua_state, 1, 0);

	// Load my benchmark library/module
	lua_pushcfunction(lua_state, luaopen_callbenchmark);
	lua_pushliteral(lua_state, "callbenchmark");
	lua_call(lua_state, 1, 0);




	asset_manager = AAssetManager_fromJava(env, java_asset_manager);
	file_asset = AAssetManager_open(asset_manager, "benchmark.lua", AASSET_MODE_UNKNOWN);
	if (NULL == file_asset)
	{
		__android_log_print(ANDROID_LOG_ERROR, "Hello-JNI", "_ASSET_NOT_FOUND_");
		is_good = JNI_FALSE;
		return is_good;
	}

	file_size = AAsset_getLength(file_asset);
	data_buffer = (char*)malloc(sizeof(char)*file_size);
	bytes_read = AAsset_read(file_asset, data_buffer, file_size);
	if(bytes_read < 0)
	{
		__android_log_print(ANDROID_LOG_ERROR, "Hello-JNI", "Could not read data from lua file");
		is_good = JNI_FALSE;		
	}
	else
	{
		ret_flag = luaL_loadbuffer(lua_state, data_buffer, bytes_read, "benchmark.lua");
		if(0 != ret_flag)
		{
			report(lua_state, ret_flag);
			is_good = JNI_FALSE;		
		}
		else
		{
			ret_flag = lua_pcall(lua_state, 0, 0, 0);
			if(0 != ret_flag)
			{
				report(lua_state, ret_flag);
				is_good = JNI_FALSE;	
			}
		}
	}
	
	free(data_buffer);
	AAsset_close(file_asset);

	return is_good;
}


void Java_com_example_hellojni_HelloJni_runBenchmarks(JNIEnv* env, jobject thiz)
{
	int elapsed_time;
	struct lua_State* lua_state;
	int ret_flag;
	lua_state = s_luaState;
	
	/* Collect full to make sure benchmark is in clean state before running. */
	lua_gc(lua_state, LUA_GCCOLLECT, 0);

	lua_getglobal(lua_state, "RunBenchmarks");
	ret_flag = lua_pcall(lua_state, 0, 0, 0);
	if(0 != ret_flag)
	{
		report(lua_state, ret_flag);
	}
//	elapsed_time = lua_tointeger(lua_state, -1);

}
