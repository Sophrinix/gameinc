#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaDevelopModule.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "LuaDevelopProject.h"

#define NO_POSTFIX
#define NO_ASSERT

namespace nrp
{
CLASS_NAME CLASS_DEVELOPMODULE( "CLuaDevelopModule" );

Luna< CLuaDevelopModule >::RegType CLuaDevelopModule::methods[] =			//���������� ������
{
	LUNA_ILUAPROJECT_HEADER( CLuaDevelopModule ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetTechGroup ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetOptionAsInt ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetEmployerPosibility ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaDevelopModule, GetParent ),
	{0,0}
};

CLuaDevelopModule::CLuaDevelopModule(lua_State *L) : ILuaProject( L, CLASS_DEVELOPMODULE )							//�����������
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

	NrpText opName = lua_tostring( L, 2 );

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = (*object_)[ opName ];
	lua_pushinteger( L, result );
	return 1;	
}

int CLuaDevelopModule::GetEmployerPosibility( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDevelopModule::GetEmployerPosibility need PUser parameter");

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
	lua_pushstring( L, GetParam_<NrpText>( L, "CLuaDevelopModule", TEXTURENORMAL, "" ) );
	return 1;
}

int CLuaDevelopModule::GetParent( lua_State* L )
{
	int argc = lua_gettop( L );
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDevelopModule:GetParent not need parameter");

	INrpDevelopProject* prj = NULL;
	IF_OBJECT_NOT_NULL_THEN prj = (*object_)[ PARENT ].As<INrpDevelopProject*>();

	lua_pop( L, argc );
	lua_pushlightuserdata( L, prj );
	Luna< CLuaDevelopProject >::constructor( L );

	return 1;
}

const char* CLuaDevelopModule::ClassName()
{
	return ( CLASS_DEVELOPMODULE );
}
}//namespace nrp