#include "StdAfx.h"
#include <string>
#include <assert.h>
#include "LuaUser.h"
#include "StrConversation.h"
#include "IUser.h"

using namespace irr;

namespace nrp
{

Luna< CLuaUser >::RegType CLuaUser::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaUser ),
	/*  */
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetSkill ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetCharacter ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetTypeName ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetParam ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetParam ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, IsTypeAs ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetName ),
	{0,0}
};

CLuaUser::CLuaUser(lua_State *L) : ILuaObject(L, "CLuaUser") {}

int CLuaUser::SetSkill( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:SetSkill need skillname, int parameter" );

	int skilltype = lua_tointeger(L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->SetSkill( skilltype, valuel );
	return 1;
}

int CLuaUser::SetCharacter( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:SetCharacter need int parameter" );

	int valuel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->SetOption<int>( CHARACTER, valuel );
	return 1;	
}

int CLuaUser::GetTypeName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetTypeName not need parameter" );
	
	std::string name = "";

	IF_OBJECT_NOT_NULL_THEN name = object_->GetType();

	lua_pushstring( L, name.c_str() );
	return 1;	
}

int CLuaUser::GetParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetParam need string parameter" );

	const char* name = lua_tostring( L, 2 );
	int valuel = 0;
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN valuel = object_->GetOption<int>( name );

	lua_pushinteger( L, valuel );
	return 1;	
}

int CLuaUser::IsTypeAs( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:IsTypeAs need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	bool result = false;

	std::string tname = object_->GetType();
	IF_OBJECT_NOT_NULL_THEN result = ( tname == std::string(name));

	lua_pushboolean( L, result );
	return 1;	
}

int CLuaUser::GetName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetName not need parameter" );

	std::string name = "";

	IF_OBJECT_NOT_NULL_THEN name = object_->GetOption<std::string>( NAME );

	lua_pushstring( L, name.c_str() );
	return 1;	
}

int CLuaUser::SetParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:SetParam need string, int parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->SetSkill( name, valuel );
	return 1;		
}
}//namespace nrp