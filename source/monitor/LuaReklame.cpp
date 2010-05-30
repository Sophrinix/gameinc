#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaReklame.h"
#include "NrpReklameWork.h"
#include "NrpPlant.h"

namespace nrp
{

Luna< CLuaReklame >::RegType CLuaReklame::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaReklame ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetTypeName ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetNumberDay ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, Create ),
	{0,0}
};

CLuaReklame::CLuaReklame(lua_State *L) : ILuaProject( L, CLASS_LUAREKLAME )							//конструктор
{}

int CLuaReklame::GetQuality( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaReklame::GetQuality not need parameter");

	float result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<float>( QUALITY );

	lua_pushnumber( L, result );
	return 1;		
}

int CLuaReklame::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaReklame:CreateTechnology need string, string parameter" );

	const char* typeName = lua_tostring( L, 2 );
	assert( typeName != NULL );
	const char* gameName = lua_tostring( L, 3 );

	object_ = CNrpPlant::Instance().CreateReklame( typeName, gameName );
	lua_pushlightuserdata(L, object_ );

	return 1;
}

int CLuaReklame::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaReklame::Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete object_;
		object_ = NULL;
	}

	return 1;	
}

int CLuaReklame::GetLevel( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaReklame::GetLevel not need parameter");

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<int>( LEVEL );

	lua_pushinteger( L, result );
	return 1;		
}

int CLuaReklame::GetNumberDay( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaReklame::GetNumberDay not need parameter");

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<int>( NUMBERDAY );

	lua_pushinteger( L, result );
	return 1;		
}

int CLuaReklame::GetTypeName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaReklame::GetNumberDay not need parameter");

	std::string result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<std::string>( TECHTYPE );

	lua_pushstring( L, result.c_str() );
	return 1;		
}
}//namespace nrp