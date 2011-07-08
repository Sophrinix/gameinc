#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaTechnology.h"
#include "NrpTechnology.h"
#include "IUser.h"
#include "NrpLinkHolder.h"
#include "LuaCompany.h"
#include "OpFileSystem.h"
#include "NrpCompany.h"

#define NO_POSTFIX
#define NO_ASSERT

namespace nrp
{
CLASS_NAME CLASS_LUATECH( L"CLuaTech" );

BEGIN_LUNA_METHODS(CLuaTechnology)
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEngineTechRequire )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEmployerSkillRequire )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetOptionAsInt )
//	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Load )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Remove )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Create )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetFutureTechInternalName )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaTechnology)
	LUNA_ILUAPROJECT_PROPERTIES(CLuaTechnology)
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "company", GetCompany, SetCompany )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "status", GetStatus, SetStatus )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "description", GetDescriptionLink, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "techGrpoup", GetTechGroup, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "baseCode", PureFunction, SetBaseCode )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "quality", PureFunction, SetQuality )
    LUNA_AUTONAME_PROPERTY( CLuaTechnology, "interest", GetInterest, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "ftNumber", GetFutureTechNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "engineCode", PureFunction, SetAddingEngineCode )
	LUNA_AUTONAME_PROPERTY( CLuaTechnology, "haveRequireTech", HaveRequireTech, PureFunction )
END_LUNA_PROPERTIES

CLuaTechnology::CLuaTechnology(lua_State *L, bool ex) : ILuaProject( L, CLASS_LUATECH, ex )							//конструктор
{}

int CLuaTechnology::GetTechGroup( lua_State* L ) 
{ 
	lua_pushinteger( L, GetParam_<int>( L, PROP, TECHGROUP, 0 ) ); 
	return 1; 
}

int CLuaTechnology::GetInterest( lua_State* L ) 
{ 
    lua_pushnumber( L, GetParam_<float>( L, PROP, INTEREST, 0 ) ); 
    return 1; 
}

int CLuaTechnology::SetBaseCode( lua_State* L )
{
	return SetParam_<float, lua_Number>( L, PROP, BASE_CODE, lua_tonumber );
}

int CLuaTechnology::SetAddingEngineCode( lua_State* L )
{
	return SetParam_<float, lua_Number>( L, PROP, ENGINE_CODE, lua_tonumber );
}

int CLuaTechnology::SetQuality( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, PROP, QUALITY, lua_tointeger );
}

int CLuaTechnology::GetOptionAsInt( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::GetOptionAsInt need int parameter");

	NrpText opName = lua_tostring( L, 2 );

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = (*_object)[ opName ];
	lua_pushinteger( L, result );
	return 1;	
}

int CLuaTechnology::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:CreateTechnology need int parameter" );

	if( lua_isnumber( L, 2 ) )
	{
		int typen = lua_tointeger( L, 2 );

		_object = new CNrpTechnology( PROJECT_TYPE(typen) );
		lua_pushlightuserdata(L, _object );
		Luna< CLuaTechnology >::constructor( L );
	}
	else if( lua_isstring( L, 2 ) )
	{
		NrpText fileName = lua_tostring( L, 2 );
		if( !OpFileSystem::IsExist( fileName ) )
			fileName = _nrpLinks.Get( fileName );

		_object = new CNrpTechnology( PROJECT_TYPE(0) );
		(*_object)[ STATUS ] = static_cast< int >( TS_READY );
		_object->Load( fileName );

		lua_pushlightuserdata(L, _object );
		Luna< CLuaTechnology >::constructor( L );
	}
	else
		lua_pushnil( L );

	return 1;
}

int CLuaTechnology::SetEngineTechRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEngineTechRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->SetEngineTechRequire( typen, valuel );

	return 0;	
}

int CLuaTechnology::SetEmployerSkillRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEmployerSkillRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->SetEmployerSkillRequire( typen, valuel );

	return 0;	
}

int CLuaTechnology::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete _object;
		_object = NULL;
	}

	return 0;	
}

int CLuaTechnology::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::Load not need parameter");

	NrpText name = lua_tostring( L, 2 );
	if( !OpFileSystem::IsExist( name ) )
		name = _nrpLinks.Get( name );

	assert( name.size() );
	IF_OBJECT_NOT_NULL_THEN	_object->Load( name );
	return 0;	
}

int CLuaTechnology::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetLevel", LEVEL, 0 ) );
	return 1;
}

int CLuaTechnology::HaveRequireTech( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpTechnology>( L, PROP, REQUIRETECH, NULL) != NULL );
	return 1;
}

int CLuaTechnology::GetFutureTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, NEXTTECHNUMBER, 0) );
	return 1;
}

int CLuaTechnology::GetFutureTechInternalName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::GetFutureTech need index parameter");

	int index = lua_tointeger( L, 2 );
	NrpText name;
	IF_OBJECT_NOT_NULL_THEN
	{
		name = _object->GetFutureTech( index );
	}

	lua_pushstring( L, name );
	return 1;	
}

int CLuaTechnology::SetStatus( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, PROP, STATUS, lua_tointeger );
}

int CLuaTechnology::GetStatus( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, STATUS, (int)TS_UNKNOWN) );
	return 1;
}

int CLuaTechnology::GetDescriptionLink( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, PROP, DESCRIPTIONPATH, "" ) );
	return 1;
}

int CLuaTechnology::GetCompany( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpCompany* cmp = (*_object)[ PARENTCOMPANY ].As<CNrpCompany*>();

		//lua_pop( L, lua_gettop(L) );
		lua_pushlightuserdata( L, cmp );
		Luna< CLuaCompany >::constructor( L );
		return 1;
	}

	lua_pushnil( L ); 
	return 1;		
}

int CLuaTechnology::SetCompany( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		CNrpCompany* cmp = _GetLuaObject< CNrpCompany, CLuaCompany >( L, -1, false );
		assert( cmp );
		(*_object)[ PARENTCOMPANY ] = cmp;
	}

	return 0;
}

const char* CLuaTechnology::ClassName()
{
	return ( CLASS_LUATECH );
}
}//namespace nrp