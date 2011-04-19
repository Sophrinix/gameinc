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
#include "NrpConfigModificator.h"
#include "NrpTimework.h"
#include "NrpLearning.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAUSER( "CLuaUser" );

BEGIN_LUNA_METHODS(CLuaUser)
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetSkill )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetSkill )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetParam )
	LUNA_AUTONAME_FUNCTION( CLuaUser, SetParam )
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddParam )
	LUNA_AUTONAME_FUNCTION( CLuaUser, IsTypeAs )
	LUNA_AUTONAME_FUNCTION( CLuaUser, Save )
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddWork )
	LUNA_AUTONAME_FUNCTION( CLuaUser, RemoveWork )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetWork )
	LUNA_AUTONAME_FUNCTION( CLuaUser, Create )
	LUNA_AUTONAME_FUNCTION( CLuaUser, GetRelation )
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddModificator )
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddLearning )
	LUNA_AUTONAME_FUNCTION( CLuaUser, AddTimeWork )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaUser)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaUser )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "name", GetName, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "typeName", GetTypeName, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "character", PureFunction, SetCharacter )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "workNumber", GetWorkNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "texture", GetTexture, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "freeUser", IsFreeUser, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "haveInvention", HaveInvention, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "yetLearning", YetLearning, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "quality", GetQuality, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "knowledges", GetKnowledges, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "qualityAward", GetQualityAward, SetQualityAward )
	LUNA_AUTONAME_PROPERTY( CLuaUser, "salary", GetSalary, SetSalary )
END_LUNA_PROPERTIES

CLuaUser::CLuaUser(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUAUSER, ex) {}

int CLuaUser::AddModificator( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaUser:AddModificator need name, days, absolute, value, modName  parameter" );

	NrpText name = lua_tostring( L, 2 );
	int days = lua_tointeger( L, 3 );
	bool absVal = lua_toboolean( L, 4 ) != 0;
	int val = lua_tointeger( L, 5 );
	NrpText modName = lua_tostring( L, 6 );

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpTime newTime = _nrpApp[ CURRENTTIME ];
		newTime.AppendDay( days );
		_object->AddModificator( new CNrpConfigModificator<int>( _object, newTime, name, absVal, val, modName ) );
		return 0;
	}

	lua_pushnil( L );
	return 0;
}

int CLuaUser::AddTimeWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaUser:AddTimeWork need days, workName, duration offset, paramName, offset parameter" );

	int days = lua_tointeger(L, 2 );
	NrpText workName = lua_tostring( L, 3 );
	int duration = lua_tointeger( L, 4 );
	NrpText paramName = lua_tostring( L, 5 );
	int offset = lua_tointeger( L, 6 );

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpTime time = _nrpApp[ CURRENTTIME ].As<NrpTime>();
		NrpTimework* we = new NrpTimework( *_object, NrpTime( time ), time.AppendDay( days ) );
		(*we)[ PARAMNAME ] = paramName;
		(*we)[ DURATION ] = duration;
		(*we)[ OFFSET ] = offset;
		(*we)[ NAME ] = workName;
		_object->AddWork( *we, true );
	}
	return 1;
}

int CLuaUser::AddLearning( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaUser:AddLearning need text, paramName, duration, offset parameter" );

	NrpText workName = lua_tostring(L, 2 );
	NrpText paramName = lua_tostring( L, 3 );
	int duration = lua_tointeger( L, 4 );
	int offset = lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpTime time = _nrpApp[ CURRENTTIME ].As<NrpTime>();
		NrpLearning* we = new NrpLearning( *_object, NrpTime( time ), time.AppendDay( duration ) );
		(*we)[ WORKNAME ] = workName;
		(*we)[ PARAMNAME ] = paramName;
		(*we)[ OFFSET ] = offset;
		(*we)[ NAME ] = workName;
		_object->AddWork( *we, true );
	}
	return 1;
}

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
		_object = new CNrpUser( userType, name );
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
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	(*_object)[ CHARACTER ] = lua_tointeger( L, -1 );

	return 0;	
}

int CLuaUser::SetQualityAward( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	(*_object)[ CHARACTER ] = lua_tointeger( L, -1 );

	return 0;	
}

int CLuaUser::GetTypeName( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText name = (*_object)[ TECHGROUP ];
		lua_pushstring( L, name );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaUser::GetKnowledges( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, (int)(*_object)[ KNOWLEDGE_LEVEL ] );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaUser::GetSalary( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, (int)(*_object)[ SALARY ] );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaUser::SetSalary( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	(*_object)[ SALARY ] = lua_tointeger( L, -1 );

	return 0;	
}


int CLuaUser::GetQuality( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, (int)(*_object)[ WORK_QUALITY ] );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaUser::GetQualityAward( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushinteger( L, (int)(*_object)[ WORK_QUALITY_AWARD ] );
		return 1;
	}

	lua_pushnil( L );
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
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText name = (NrpText)(*_object)[ NAME ];
		lua_pushstring( L, name );
		return 1;
	}

	lua_pushnil( L );
	return 1;	
}

int CLuaUser::SetParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaUser:SetParam need name, integer parameter" );

	NrpText name = lua_tostring( L, 2 );
	int addvalue = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN  
	{
		assert( _object->IsExist( name ) );
		if( _object->IsExist( name ) )
			(*_object)[ name ] = addvalue;
	}

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

	IWorkingModule* work = _GetLuaObject< IWorkingModule, ILuaObject >( L, 2, true );
	assert( work != NULL );

	IF_OBJECT_NOT_NULL_THEN 
	{
		if( work )
			_object->AddWork( *work );
	}

	return 0;		
}

int CLuaUser::GetWorkNumber( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		int valuel = (*_object)[ WORKNUMBER ];
		lua_pushinteger( L, valuel );
		return 1;		
	}

	lua_pushnil( L );
	return 1;
}

int CLuaUser::RemoveWork( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:AddTechWork need CNrpTechnology* parameter" );

	IWorkingModule* work = _GetLuaObject< IWorkingModule, ILuaObject >( L, 2, true );
	assert( work );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( work )
			_object->RemoveWork( *work );
	}

	return 0;		
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
	IF_OBJECT_NOT_NULL_THEN 
	{
		NrpText pathName = (*_object)[ TEXTURENORMAL ];
		lua_pushstring( L, pathName );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaUser::IsFreeUser( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		bool noCompany = (*_object)[ PARENTCOMPANY ].As<PNrpCompany>() == NULL;
		lua_pushboolean( L, noCompany );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaUser::GetRelation( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaUser:GetRelation need name of relation parameter" );

	NrpText name( lua_tostring(L, 2 ) );
	CNrpRelation* ret = 0;

	IF_OBJECT_NOT_NULL_THEN ret = _object->GetRelation( name );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, ret );
	Luna< CLuaRelation >::constructor( L );

	return 1;	
}

template< class T >
bool _FindWork( CNrpUser* user )
{
	for( int k=0; k < (int)(*user)[ WORKNUMBER ]; k++ )
		if( user->GetWork( k )->ObjectTypeName() == T::ClassName() )
			return true;

	return false;
}

int CLuaUser::HaveInvention( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{	
		lua_pushboolean( L, _FindWork< CNrpInvention >( _object ) );
		return 1;
	}

	lua_pushnil( L );
	return 1;		
}

int CLuaUser::AddParam( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaUser:AddParam need integer parameter" );

	NrpText name = lua_tostring( L, 2 );
	int addvalue = lua_tointeger( L, 3 );
	bool createValue = lua_toboolean( L, 4 ) != 0;

	IF_OBJECT_NOT_NULL_THEN 
	{
		if( !_object->IsExist( name ) && createValue )
			_object->Add<int>( name, 0 );

		(*_object)[ name ] += addvalue;
	}

	return 1;	
}

const char* CLuaUser::ClassName()
{
	return ( CLASS_LUAUSER );
}

int CLuaUser::YetLearning( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		lua_pushboolean( L, _FindWork< NrpLearning >( _object ) );
		return 1;
	}

	lua_pushnil( L );
	return 1;			
}

}//namespace nrp