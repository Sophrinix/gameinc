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

BEGIN_LUNA_METHODS(CLuaUser)
	LUNA_ILUAOBJECT_HEADER( CLuaUser )
	/*  */
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetSkill )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetSkill )
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetCharacter )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetTypeName )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetParam )
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetParam )
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddParam )
	LUNA_AUTONAME_FUNCTION( CLuaUser, IsTypeAs )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetName )
	LUNA_AUTONAME_FUNCTION( CLuaUser, Save )
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddWork )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetWorkNumber )
	LUNA_AUTONAME_FUNCTION( CLuaUser, RemoveWork )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetWork )
	LUNA_AUTONAME_FUNCTION( CLuaUser, Create )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaUser, IsFreeUser )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetRelation )
	LUNA_AUTONAME_FUNCTION( CLuaUser, HaveInvention )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaUser)
END_LUNA_PROPERTIES

CLuaUser::CLuaUser(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUAUSER, ex) {}

int CLuaUser::Create( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:Create need 2 parameter" );

	NrpText userType( lua_tostring( L, 2 ) );
	NrpText name( lua_tostring( L, 3 ) );

	if( userType == CNrpPlayer::ClassName() )
	{
		_object = new CNrpPlayer( name, NULL );
		_nrpApp.AddUser( _object );
	}
	else if( userType == CNrpAiUser::ClassName() )
	{
		_object = new CNrpAiUser( name, NULL );
		_nrpApp.AddUser( _object );
	}
	else 
	{
		_object = new IUser( userType, name );
		_nrpApp.AddUser( _object );
	}

	lua_pushlightuserdata( L, (void*)_object );
	return 1;
}

int CLuaUser::SetSkill( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:SetSkill need skillname, int parameter" );

	int skilltype = lua_tointeger(L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN _object->SetSkill( skilltype, valuel );
	return 1;
}

int CLuaUser::SetCharacter( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:SetCharacter need int parameter" );

	int valuel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN (*_object)[ CHARACTER ] = valuel;
	return 1;	
}

int CLuaUser::GetTypeName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetTypeName not need parameter" );
	
	NrpText name;

	IF_OBJECT_NOT_NULL_THEN name = (*_object)[ TECHGROUP ];

	lua_pushstring( L, name );
	return 1;	
}

int CLuaUser::GetParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetParam need string parameter" );

	NrpText name = lua_tostring( L, 2 );
	int valuel = 0;

	IF_OBJECT_NOT_NULL_THEN valuel = (*_object)[ name ];
 
	lua_pushinteger( L, valuel );
	return 1;	
}

int CLuaUser::IsTypeAs( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:IsTypeAs need string parameter" );

	NrpText name( lua_tostring( L, 2 ) );
	bool result = false;

	IF_OBJECT_NOT_NULL_THEN result = ( _object->ObjectTypeName() == name );

	lua_pushboolean( L, result );
	return 1;	
}

int CLuaUser::GetName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetName not need parameter" );

	NrpText name;

	IF_OBJECT_NOT_NULL_THEN name = (NrpText)(*_object)[ NAME ];

	lua_pushstring( L, name );
	return 1;	
}

int CLuaUser::SetParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:SetParam need string, int parameter" );

	NrpText name( lua_tostring( L, 2 ) );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN _object->SetSkill( name, valuel );
	return 1;		
}

int CLuaUser::GetSkill( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetSkill need skillname parameter" );

	NrpText skillName = lua_tostring(L, 2 );
	int valuel = 0;

	IF_OBJECT_NOT_NULL_THEN valuel = _object->GetSkill( skillName );

	lua_pushinteger( L, valuel );
	return 1;
}

int CLuaUser::Save( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetName need string parameter" );

	NrpText fileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->Save( fileName );

	return 1;	
}

int CLuaUser::AddWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:AddTechWork need CNrpTechnology* parameter" );

	IWorkingModule* work = (IWorkingModule*)lua_touserdata( L, 2 );
	assert( work != NULL );

	IF_OBJECT_NOT_NULL_THEN _object->AddWork( work );

	return 1;		
}

int CLuaUser::GetWorkNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetName not need parameter" );

	int valuel = 0;

	IF_OBJECT_NOT_NULL_THEN valuel = (*_object)[ WORKNUMBER ];

	lua_pushinteger( L, valuel );
	return 1;		
}

int CLuaUser::RemoveWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:AddTechWork need CNrpTechnology* parameter" );

	IWorkingModule* work = (IWorkingModule*)lua_touserdata( L, 2 );
	assert( work != NULL );

	IF_OBJECT_NOT_NULL_THEN _object->RemoveWork( work );

	return 1;		
}

int CLuaUser::GetWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetSkill need index of techWork parameter" );

	int index = lua_tointeger(L, 2 );
	IWorkingModule* work = NULL;

	IF_OBJECT_NOT_NULL_THEN work = _object->GetWork( index );

	lua_pushlightuserdata( L, work );
	return 1;	
}

int CLuaUser::GetTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:GetTexture not need any parameter" );

	NrpText pathName = "";
	IF_OBJECT_NOT_NULL_THEN pathName = (*_object)[ TEXTURENORMAL ];

	lua_pushstring( L, pathName );
	return 1;		
}

int CLuaUser::IsFreeUser( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaUser:IsFreeUser not need any parameter" );

	bool noCompany = false;
	IF_OBJECT_NOT_NULL_THEN noCompany = (*_object)[ PARENTCOMPANY ].As<PNrpCompany>() == NULL;

	lua_pushboolean( L, noCompany );
	return 1;		
}

int CLuaUser::GetRelation( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetRelation need name of relation parameter" );

	NrpText name( lua_tostring(L, 2 ) );
	CNrpRelation* ret = 0;

	IF_OBJECT_NOT_NULL_THEN ret = _object->GetRelation( name );

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
		for( int k=0; k < (int)(*_object)[ WORKNUMBER ]; k++ )
		{
			if( _object->GetWork( k )->ObjectTypeName() == CNrpInvention::ClassName() )
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

	NrpText name = lua_tostring( L, 2 );
	int addvalue = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		(*_object)[ name ] += addvalue;
	}

	return 1;	
}

const char* CLuaUser::ClassName()
{
	return ( CLASS_LUAUSER );
}
}//namespace nrp