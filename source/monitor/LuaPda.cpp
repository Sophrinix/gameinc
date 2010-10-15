#include "StdAfx.h"
#include "LuaPda.h"
#include "NrpApplication.h"
#include "NrpPda.h"

namespace nrp
{

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

	std::string ret = "0:0_error";
	IF_OBJECT_NOT_NULL_THEN ret = object_->Current().GetValue<std::string>( MESSAGE );

	lua_pushstring( L, ret.c_str() );
	return 1;
}

int CLuaPda::Next( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Next not need parameter");

	IF_OBJECT_NOT_NULL_THEN object_->Next();

	return 1;
}

int CLuaPda::Prev( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Prev not need parameter");

	IF_OBJECT_NOT_NULL_THEN object_->Prev();

	return 1;
}

int CLuaPda::GetTimeStr( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::GetMessage not need parameter");

	std::string ret = "0:0_error";
	IF_OBJECT_NOT_NULL_THEN
	{
		SYSTEMTIME time = object_->Current().GetValue<SYSTEMTIME>( STARTDATE );
		char dd[ MAX_PATH ] = { 0 };
		snprintf( dd, MAX_PATH - 1, "%04d.%02d.%02d %02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute );
		ret = dd;
	}

	lua_pushstring( L, ret.c_str() );
	return 1;
}

int CLuaPda::AddMessage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaPda::AddMessage need message, action parameter");

	const char* message = lua_tostring( L, 2 );
	const char* action = lua_tostring( L, 3 );

	assert( action != NULL && message != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( action != NULL && message != NULL )
		{
			SYSTEMTIME time = CNrpApplication::Instance().GetValue<SYSTEMTIME>( CURRENTTIME );
			object_->AddItem( message, action, time );
		}
	}

	return 1;
}

int CLuaPda::Save( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Save not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		std::string saveFile = CNrpApplication::Instance().GetValue<std::string>( SAVEDIR );
		saveFile += CNrpApplication::Instance().GetValue<std::string>( PROFILENAME );
		saveFile += "/pda.ini";
		object_->Save( saveFile );
	}

	return 1;
}	

int CLuaPda::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaPda::Load not need parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		std::string saveFile = CNrpApplication::Instance().GetValue<std::string>( SAVEDIR );
		saveFile += CNrpApplication::Instance().GetValue<std::string>( PROFILENAME );
		saveFile += "/pda.ini";
		object_->Load( saveFile );
	}

	return 1;
}

}//namespace nrp