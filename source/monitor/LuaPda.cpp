#include "StdAfx.h"
#include "LuaPda.h"
#include "NrpApplication.h"
#include "NrpPda.h"

namespace nrp
{
CLASS_NAME CLASS_LUAPDA( "CLuaPda" );

Luna< CLuaPda >::RegType CLuaPda::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaPda ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaPda, GetMessage ),
	LUNA_AUTONAME_FUNCTION( CLuaPda, GetTimeStr ),
	LUNA_AUTONAME_FUNCTION( CLuaPda, Next ),
	LUNA_AUTONAME_FUNCTION( CLuaPda, Prev ),
	LUNA_AUTONAME_FUNCTION( CLuaPda, AddMessage ),
	LUNA_AUTONAME_FUNCTION( CLuaPda, Save ),
	LUNA_AUTONAME_FUNCTION( CLuaPda, Load ),
	{0,0}
};

CLuaPda::CLuaPda(lua_State *L) : ILuaObject(L, "CLuaPda")
{}

int CLuaPda::GetMessage( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::GetMessage not need parameter");

	NrpText ret = "0:0_error";
	IF_OBJECT_NOT_NULL_THEN ret = const_cast< CPdaItem& >( _object->Current() )[ MESSAGE ];

	lua_pushstring( L, ret );
	return 1;
}

int CLuaPda::Next( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Next not need parameter");

	IF_OBJECT_NOT_NULL_THEN _object->Next();

	return 1;
}

int CLuaPda::Prev( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Prev not need parameter");

	IF_OBJECT_NOT_NULL_THEN _object->Prev();

	return 1;
}

int CLuaPda::GetTimeStr( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::GetMessage not need parameter");

	NrpText ret = "0:0_error";
	IF_OBJECT_NOT_NULL_THEN
	{
		SYSTEMTIME time = _object->Current()[ STARTDATE ].As<SYSTEMTIME>();
		char dd[ MAX_PATH ] = { 0 };
		snprintf( dd, MAX_PATH - 1, "%04d.%02d.%02d %02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute );
		ret = dd;
	}

	lua_pushstring( L, ret );
	return 1;
}

int CLuaPda::AddMessage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPda::AddMessage need message, action parameter");

	NrpText message = lua_tostring( L, 2 );
	NrpText action = lua_tostring( L, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		SYSTEMTIME time = _nrpApp[ CURRENTTIME ].As<SYSTEMTIME>();
		_object->AddItem( message, action, time );
	}

	return 1;
}

int CLuaPda::Save( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Save not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		NrpText saveFile =_nrpApp[ SAVEDIR ];
		saveFile.append( (NrpText)_nrpApp[ PROFILENAME ] );
		saveFile.append( L"/pda.ini" );

		_object->Save( saveFile );
	}

	return 1;
}	

int CLuaPda::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Load not need parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText saveFile = _nrpApp[ SAVEDIR ];
		saveFile.append( (NrpText)_nrpApp[ PROFILENAME ] );
		saveFile.append( L"/pda.ini" );
		_object->Load( saveFile );
	}

	return 1;
}

const char* CLuaPda::ClassName()
{
	return ( CLASS_LUAPDA );
}

}//namespace nrp