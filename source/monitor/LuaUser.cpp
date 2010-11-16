#include "StdAfx.h"
#include <assert.h>
#include "LuaUser.h"
#include "NrpText.h"
#include "IUser.h"
#include "NrpPlayer.h"
#include "NrpApplication.h"
#include "NrpAiUser.h"
#include "NrpTechnology.h"
#include "NrpCompany.h"
#include "LuaRelation.h"
#include "IWorkingModule.h"
#include "NrpInvention.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAUSER( "CLuaUser" );

Luna< CLuaUser >::RegType CLuaUser::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaUser ),
	/*  */
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetSkill ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetSkill ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetCharacter ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetTypeName ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetParam ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetParam ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddParam ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, IsTypeAs ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, Save ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddWork ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetWorkNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, RemoveWork ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetWork ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, IsFreeUser ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetRelation ),
	LUNA_AUTONAME_FUNCTION( CLuaUser, HaveInvention ),
	{0,0}
};

CLuaUser::CLuaUser(lua_State *L) : ILuaObject(L, "CLuaUser") {}

int CLuaUser::Create( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:Create need 2 parameter" );

	NrpText userType( lua_tostring( L, 2 ) );
	NrpText name( lua_tostring( L, 3 ) );

	if( userType == CNrpPlayer::ClassName() )
	{
		object_ = new CNrpPlayer( name, NULL );
		CNrpApplication::Instance().AddUser( object_ );
	}
	else if( userType == CNrpAiUser::ClassName() )
	{
		object_ = new CNrpAiUser( name, NULL );
		CNrpApplication::Instance().AddUser( object_ );
	}
	else 
	{
		object_ = new IUser( userType, name );
		CNrpApplication::Instance().AddUser( object_ );
	}

	lua_pushlightuserdata( L, (void*)object_ );
	return 1;
}

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

	IF_OBJECT_NOT_NULL_THEN object_->SetValue<int>( CHARACTER, valuel );
	return 1;	
}

int CLuaUser::GetTypeName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetTypeName not need parameter" );
	
	NrpText name;

	IF_OBJECT_NOT_NULL_THEN name = object_->ObjectTypeName();

	lua_pushstring( L, name );
	return 1;	
}

int CLuaUser::GetParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetParam need string parameter" );

	const char* name = lua_tostring( L, 2 );
	int valuel = 0;
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN valuel = object_->GetValue<int>( name );

	lua_pushinteger( L, valuel );
	return 1;	
}

int CLuaUser::IsTypeAs( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:IsTypeAs need string parameter" );

	NrpText name( lua_tostring( L, 2 ) );
	bool result = false;

	IF_OBJECT_NOT_NULL_THEN result = ( object_->ObjectTypeName() == name );

	lua_pushboolean( L, result );
	return 1;	
}

int CLuaUser::GetName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetName not need parameter" );

	NrpText name;

	IF_OBJECT_NOT_NULL_THEN name = object_->GetValue<NrpText>( NAME );

	lua_pushstring( L, name );
	return 1;	
}

int CLuaUser::SetParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:SetParam need string, int parameter" );

	NrpText name( lua_tostring( L, 2 ) );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN object_->SetSkill( name, valuel );
	return 1;		
}

int CLuaUser::GetSkill( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetSkill need skillname parameter" );

	int skilltype = lua_tointeger(L, 2 );
	int valuel = 0;

	IF_OBJECT_NOT_NULL_THEN valuel = object_->GetSkill( skilltype );

	lua_pushinteger( L, valuel );
	return 1;
}

int CLuaUser::Save( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetName need string parameter" );

	const char* fileName = lua_tostring( L, 2 );
	assert( fileName != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->Save( fileName );

	return 1;	
}

int CLuaUser::AddWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:AddTechWork need CNrpTechnology* parameter" );

	IWorkingModule* work = (IWorkingModule*)lua_touserdata( L, 2 );
	assert( work != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->AddWork( work );

	return 1;		
}

int CLuaUser::GetWorkNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetName not need parameter" );

	int valuel = 0;

	IF_OBJECT_NOT_NULL_THEN valuel = object_->GetValue<int>( WORKNUMBER );

	lua_pushinteger( L, valuel );
	return 1;		
}

int CLuaUser::RemoveWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:AddTechWork need CNrpTechnology* parameter" );

	IWorkingModule* work = (IWorkingModule*)lua_touserdata( L, 2 );
	assert( work != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->RemoveWork( work );

	return 1;		
}

int CLuaUser::GetWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetSkill need index of techWork parameter" );

	int index = lua_tointeger(L, 2 );
	IWorkingModule* work = NULL;

	IF_OBJECT_NOT_NULL_THEN work = object_->GetWork( index );

	lua_pushlightuserdata( L, work );
	return 1;	
}

int CLuaUser::GetTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetTexture not need any parameter" );

	NrpText pathName = "";
	IF_OBJECT_NOT_NULL_THEN pathName = object_->GetString( TEXTURENORMAL );

	lua_pushstring( L, pathName );
	return 1;		
}

int CLuaUser::IsFreeUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:IsFreeUser not need any parameter" );

	bool noCompany = false;
	IF_OBJECT_NOT_NULL_THEN noCompany = object_->GetValue<PNrpCompany>( PARENTCOMPANY ) == NULL;

	lua_pushboolean( L, noCompany );
	return 1;		
}

int CLuaUser::GetRelation( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetRelation need name of relation parameter" );

	NrpText name( lua_tostring(L, 2 ) );
	CNrpRelation* ret = 0;

	IF_OBJECT_NOT_NULL_THEN ret = object_->GetRelation( name );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, ret );
	Luna< CLuaRelation >::constructor( L );

	return 1;	
}

int CLuaUser::HaveInvention( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:HaveInvention not need any parameter" );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN 
	{
		for( int k=0; k < object_->GetValue<int>( WORKNUMBER ); k++ )
		{
			if( object_->GetWork( k )->ObjectTypeName() == CNrpInvention::ClassName() )
			{
				ret = true;
				break;
			}
		}
	}

	lua_pushboolean( L, ret );
	return 1;		
}

int CLuaUser::AddParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:AddParam need integer parameter" );

	const char* name = lua_tostring( L, 2 );
	int addvalue = lua_tointeger( L, 3 );

	assert( name != NULL );
	IF_OBJECT_NOT_NULL_THEN 
	{
		object_->AddValue<int>( name, addvalue );
	}

	return 1;	
}

const char* CLuaUser::ClassName()
{
	return ( CLASS_LUAUSER );
}
}//namespace nrp