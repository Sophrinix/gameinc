#include "StdAfx.h"
#include "LuaPda.h"
#include "NrpApplication.h"
#include "NrpPda.h"

namespace nrp
{
CLASS_NAME CLASS_LUAPDA( "CLuaPda" );

BEGIN_LUNA_METHODS(CLuaPda)
	LUNA_AUTONAME_FUNCTION( CLuaPda, Next )
	LUNA_AUTONAME_FUNCTION( CLuaPda, Prev )
	LUNA_AUTONAME_FUNCTION( CLuaPda, AddMessage )
	LUNA_AUTONAME_FUNCTION( CLuaPda, Save )
	LUNA_AUTONAME_FUNCTION( CLuaPda, Load )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaPda)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaPda )
	LUNA_AUTONAME_PROPERTY( CLuaPda, "time", GetTimeStr, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaPda, "message", GetMessage, PureFunction )
END_LUNA_PROPERTIES

CLuaPda::CLuaPda(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUAPDA, ex)
{}

int CLuaPda::GetMessage( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		NrpText ret = const_cast< CPdaItem& >( _object->Current() )[ MESSAGE ];
		lua_pushstring( L, ret );
		return 1;
	}

	lua_pushnil( L );
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
	NrpText ret = "0:0_error";
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpTime time = _object->Current()[ STARTDATE ].As<NrpTime>();
		char dd[ MAX_PATH ] = { 0 };
		snprintf( dd, MAX_PATH - 1, "%04d.%02d.%02d %02d:%02d", time.RYear(), time.RMonth(), time.RDay(), time.RHour(), time.RMinute() );
		ret = dd;
		lua_pushstring( L, ret );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaPda::AddMessage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPda::AddMessage need message, action parameter");

	NrpText message = lua_tostring( L, 2 );
	
	int refunc = -1;
	if( !lua_isnil( L, 3 ) )
		refunc = _GetRef( L, 3 );

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpTime time = _nrpApp[ CURRENTTIME ].As<NrpTime>();
		_object->AddItem( message, refunc, time );
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