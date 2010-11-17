#include "StdAfx.h"
#include "LuaIniFile.h"
#include "IniFile.h"
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_LUAINIFILE( "CLuaIniFile" );

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

CLuaIniFile::CLuaIniFile(lua_State *L)	: ILuaObject(L, CLASS_LUAINIFILE)	//конструктор
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaIniFile::CLuaIniFile need nil, string parameter");

	NrpText fn = lua_tostring( L, 2 );

	_ini = new IniFile( fn );
}

int CLuaIniFile::ReadInteger( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaIniFile::ReadInteger need string parameter");

	NrpText section = lua_tostring( L, 2 );
	NrpText key = lua_tostring( L, 3 );
	int defaultValue = lua_tointeger( L, 4 );

	int resultt = 0;

	resultt = _ini->Get( section, key, (int)defaultValue );

	lua_pushinteger( L, resultt );

	return 1;
}

int CLuaIniFile::ReadString( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaIniFile::ReadString need string parameter");

	NrpText section = lua_tostring( L, 2 );
	NrpText key = lua_tostring( L, 3 );
	NrpText defaultValue = lua_tostring( L, 4 );
	NrpText resultt;
	resultt = _ini->Get( section, key, defaultValue );

	lua_pushstring( L, resultt );
	return 1;
}

int CLuaIniFile::ReadFloat( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaIniFile::ReadFloat need string parameter");

	NrpText section = lua_tostring( L, 2 );
	NrpText key = lua_tostring( L, 3 );
	float defaultValue = (float)lua_tonumber( L, 4 );

	float resultt = 0;
	resultt = _ini->Get( section, key, defaultValue );

	lua_pushnumber( L, resultt );
	return 1;
}

int CLuaIniFile::ReadTime( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaIniFile::ReadFloat need string parameter");

	NrpText section = lua_tostring( L, 2 );
	NrpText key = lua_tostring( L, 3 );

	SYSTEMTIME time;
	memset( &time, 0, sizeof( SYSTEMTIME ) );

	time = _ini->Get( section, key, time );

	assert( time.wYear > 0 && time.wMonth > 0 && time.wMonth <= 12 && 
			time.wDay > 0 && time.wDay <= 31 );

	lua_pushinteger( L, time.wYear );
	lua_pushinteger( L, time.wMonth );
	lua_pushinteger( L, time.wDay );
	lua_pushinteger( L, time.wHour );
	lua_pushinteger( L, time.wMinute );

	return 5;
}

CLuaIniFile::~CLuaIniFile()
{
	delete _ini;
}

const char* CLuaIniFile::ClassName()
{
	return ( CLASS_LUAINIFILE );
}

}//namespace nrp