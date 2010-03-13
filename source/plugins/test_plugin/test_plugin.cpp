// test_plugin.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <lua.hpp>

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) void GetPluginName( char* name );
	__declspec(dllexport) void RegisterFunctions( lua_State* vm );
	__declspec(dllexport) void GetPluginVersion( int &version );

#ifdef __cplusplus
}
#endif

void GetPluginName( char* name )
{
	strcpy_s( name, 32, "test" );
}

int test_OutputDebugString( lua_State* vm )
{
	int argc = lua_gettop(vm);
	luaL_argcheck(vm, argc == 0, 0, "Function test_OutputDebugString not need any parameter");

	OutputDebugString( "Это тестовое сообщение из библиотеки test.nrp\n" );

	return 1;
}

void RegisterFunctions( lua_State* vm )
{
	lua_register( vm, "test_OutputDebugString", test_OutputDebugString );
}

void GetPluginVersion( int &version )
{
	version = 1000;
}