#include "StdAfx.h"
#include <string>
#include <assert.h>
#include "LuaUser.h"
#include "StrConversation.h"
#include "IUser.h"
#include "NrpPlayer.h"
#include "NrpApplication.h"
#include "NrpAiUser.h"
#include "NrpTechnology.h"
#include "NrpCompany.h"

using namespace irr;

namespace nrp
{

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
	{0,0}
};

CLuaUser::CLuaUser(lua_State *L) : ILuaObject(L, "CLuaUser") {}

int CLuaUser::Create( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:Create need 2 parameter" );

	const char* userType = lua_tostring( L, 2 );
	assert( userType != NULL );

	const char* name = lua_tostring( L, 3 );
	assert( name != NULL );

	if( strcmp( userType, "RealPlayer" ) == 0 )
	{
		object_ = new CNrpPlayer( name, NULL );
		CNrpApplication::Instance().AddUser( true, object_ );
	}
	else if( strcmp( userType, "AIPlayer" ) == 0 )
	{
		object_ = new CNrpAiUser( name, NULL );
		CNrpApplication::Instance().AddUser( true, object_ );
	}
	else 
	{
		object_ = new IUser( std::string(userType), std::string(name) );
		object_->SetValue<std::string>( NAME, name );
		CNrpApplication::Instance().AddUser( false, object_ );
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

	IF_OBJECT_NOT_NULL_THEN valuel = object_->GetValue<int>( name );

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

	IF_OBJECT_NOT_NULL_THEN name = object_->GetValue<std::string>( NAME );

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

	IF_OBJECT_NOT_NULL_THEN valuel = object_->GetValue<int>( TECHNUMBER );

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

	std::string pathName = "";
	IF_OBJECT_NOT_NULL_THEN pathName = object_->GetValue<std::string>( TEXTURENORMAL );

	lua_pushstring( L, pathName.c_str() );
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
}//namespace nrp