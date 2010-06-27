#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaDevelopModule.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "NrpApplication.h"

#define NO_POSTFIX
#define NO_ASSERT

namespace nrp
{

Luna< CLuaDevelopModule >::RegType CLuaDevelopModule::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaDevelopModule ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetTechGroup ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetOptionAsInt ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, HaveLider ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetEmployerPosibility ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetTexture ),
	{0,0}
};

CLuaDevelopModule::CLuaDevelopModule(lua_State *L) : ILuaProject( L, CLASS_DEVELOPMODULE )							//конструктор
{}

int CLuaDevelopModule::GetTechGroup( lua_State* L ) 
{ 
	lua_pushinteger( L, GetParam_<int>( L, "GetTechGroup", TECHGROUP, 0 ) ); 
	return 1; 
}

int CLuaDevelopModule::GetOptionAsInt( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDevelopModule::GetOptionAsInt need int parameter");

	const char* opName = lua_tostring( L, 2 );
	assert( opName != NULL );

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<int>( opName );
	lua_pushinteger( L, result );
	return 1;	
}

int CLuaDevelopModule::HaveLider( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::HaveLider not need parameter");

	bool haveUser = false; 
	IF_OBJECT_NOT_NULL_THEN	haveUser = !object_->GetValue<std::string>( COMPONENTLIDER ).empty();
	lua_pushboolean( L, haveUser );
	return 1;	
}

int CLuaDevelopModule::GetEmployerPosibility( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDevelopModule::GetEmployerPosibility not need parameter");

	float posilbleValue = 0; 
	PUser puser = (PUser)lua_touserdata( L, 2 );
	assert( puser != NULL );
	if( puser != NULL )
		IF_OBJECT_NOT_NULL_THEN	posilbleValue = object_->GetEmployerPosibility( puser );

	lua_pushnumber( L, posilbleValue );
	return 1;	
}

int CLuaDevelopModule::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDevelopModule:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete object_;
		object_ = NULL;
	}

	return 1;	
}

int CLuaDevelopModule::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "CLuaDevelopModule", LEVEL, 0 ) );
	return 1;
}

int CLuaDevelopModule::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "CLuaDevelopModule", TEXTURENORMAL, "" ).c_str() );
	return 1;
}
}//namespace nrp