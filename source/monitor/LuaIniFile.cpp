#include "StdAfx.h"
#include "LuaIniFile.h"
#include "IniFile.h"
#include <assert.h>

namespace nrp
{

Luna< CLuaIniFile >::RegType CLuaIniFile::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaIniFile ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaIniFile, ReadInteger ),
	LUNA_AUTONAME_FUNCTION( CLuaIniFile, ReadString ),
	LUNA_AUTONAME_FUNCTION( CLuaIniFile, ReadFloat ),
	LUNA_AUTONAME_FUNCTION( CLuaIniFile, ReadTime ),
	{0,0}
};

CLuaIniFile::CLuaIniFile(lua_State *L)	: ILuaObject(L, "CLuaIniFile")	//конструктор
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaIniFile::CLuaIniFile need nil, string parameter");

	const char* fn = lua_tostring( L, 2 );
	assert( fn != NULL );
	fileName_ = fn;
}

int CLuaIniFile::ReadInteger( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaIniFile::ReadInteger need string parameter");

	const char* section = lua_tostring( L, 2 );
	const char* key = lua_tostring( L, 3 );
	int defaultValue = lua_tointeger( L, 4 );
	assert( section != NULL || key == NULL );
	int resultt = 0;
	resultt = IniFile::Read( section, key, (int)defaultValue, fileName_ );

	lua_pushinteger( L, resultt );

	return 1;
}

int CLuaIniFile::ReadString( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaIniFile::ReadString need string parameter");

	const char* section = lua_tostring( L, 2 );
	const char* key = lua_tostring( L, 3 );
	const char* defaultValue = lua_tostring( L, 4 );
	assert( section != NULL || key == NULL );
	std::string resultt;
	resultt = IniFile::Read( section, key, (std::string)defaultValue, fileName_ );

	lua_pushstring( L, resultt.c_str() );

	return 1;
}

int CLuaIniFile::ReadFloat( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaIniFile::ReadFloat need string parameter");

	const char* section = lua_tostring( L, 2 );
	const char* key = lua_tostring( L, 3 );
	float defaultValue = (float)lua_tonumber( L, 4 );
	assert( section != NULL || key == NULL );
	float resultt = 0;
	resultt = IniFile::Read( section, key, defaultValue, fileName_ );

	lua_pushnumber( L, resultt );

	return 1;
}

int CLuaIniFile::ReadTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaIniFile::ReadFloat need string parameter");

	const char* section = lua_tostring( L, 2 );
	const char* key = lua_tostring( L, 3 );

	SYSTEMTIME time;
	memset( &time, 0, sizeof( SYSTEMTIME ) );
	assert( section != NULL || key == NULL );

	time = IniFile::Read( section, key, time, fileName_ );

	assert( time.wYear > 0 && time.wMonth > 0 && time.wMonth <= 12 && 
			time.wDay > 0 && time.wDay <= 31 );

	lua_pushinteger( L, time.wYear );
	lua_pushinteger( L, time.wMonth );
	lua_pushinteger( L, time.wDay );
	lua_pushinteger( L, time.wHour );
	lua_pushinteger( L, time.wMinute );

	return 5;
}


}//namespace nrp