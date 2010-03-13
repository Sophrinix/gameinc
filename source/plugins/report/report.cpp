// report.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <lua.hpp>
#include <luna.h>
#include "LuaReport.h"

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

void RegisterFunctions( lua_State* vm )
{
	Luna< CLuaReport >::Register( vm );
}

void GetPluginVersion( int &version )
{
	version = 1000;
}

