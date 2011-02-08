#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaTechnology.h"
#include "NrpTechnology.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "LuaCompany.h"
#include "OpFileSystem.h"

#define NO_POSTFIX
#define NO_ASSERT

namespace nrp
{
CLASS_NAME CLASS_LUATECH( L"CLuaTech" );

BEGIN_LUNA_METHODS(CLuaTechnology)
	LUNA_ILUAPROJECT_HEADER( CLuaTechnology )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetBaseCode )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetAddingEngineCode )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEngineTechRequire )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEmployerSkillRequire )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTechGroup )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetOptionAsInt )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetQuality )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Load )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Remove )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetLevel )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Create )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, HaveRequireTech )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetFutureTechNumber )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetFutureTechInternalName )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetStatus )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetStatus )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetDescriptionLink )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetCompany )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetCompany )
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetInternalName )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaTechnology)
END_LUNA_PROPERTIES

CLuaTechnology::CLuaTechnology(lua_State *L, bool ex) : ILuaProject( L, CLASS_LUATECH, ex )							//конструктор
{}

int CLuaTechnology::GetTechGroup( lua_State* L ) 
{ 
	lua_pushinteger( L, GetParam_<int>( L, "GetTechGroup", TECHGROUP, 0 ) ); 
	return 1; 
}

int CLuaTechnology::SetBaseCode( lua_State* L )
{
	return SetParam_<float, lua_Number>( L, "SetBaseCode", BASE_CODE, lua_tonumber );
}

int CLuaTechnology::SetAddingEngineCode( lua_State* L )
{
	return SetParam_<float, lua_Number>( L, "SetAddingEngineCode", ENGINE_CODE, lua_tonumber );
}

int CLuaTechnology::SetQuality( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, "SetQuality", QUALITY, lua_tointeger );
}

int CLuaTechnology::SetTexture( lua_State* L)
{
	return SetParam_( L, "SetTexture", TEXTURENORMAL );
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

	int typen = lua_tointeger( L, 2 );

	_object = new CNrpTechnology( PROJECT_TYPE(typen) );
	lua_pushlightuserdata(L, _object );

	return 1;
}

int CLuaTechnology::SetEngineTechRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEngineTechRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->SetEngineTechRequire( typen, valuel );

	return 1;	
}

int CLuaTechnology::SetEmployerSkillRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEmployerSkillRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->SetEmployerSkillRequire( typen, valuel );

	return 1;	
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

	return 1;	
}

int CLuaTechnology::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::Load not need parameter");

	NrpText name = lua_tostring( L, 2 );
	if( !OpFileSystem::IsExist( name ) )
		name = CNrpApplication::Instance().GetLink( name );

	assert( name.size() );
	IF_OBJECT_NOT_NULL_THEN	_object->Load( name );
	return 1;	
}

int CLuaTechnology::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetLevel", LEVEL, 0 ) );
	return 1;
}

int CLuaTechnology::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetTexture", TEXTURENORMAL, "" ) );
	return 1;
}

int CLuaTechnology::HaveRequireTech( lua_State* L )
{
	lua_pushboolean( L, GetParam_<PNrpTechnology>( L, "HaveRequireTech", REQUIRETECH, NULL) != NULL );
	return 1;
}

int CLuaTechnology::GetFutureTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetFutureTechNumber", NEXTTECHNUMBER, 0) );
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
	return SetParam_<int, lua_Integer>( L, "SetStatus", STATUS, lua_tointeger );
}

int CLuaTechnology::GetStatus( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetStatus", STATUS, (int)TS_UNKNOWN) );
	return 1;
}

int CLuaTechnology::GetDescriptionLink( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetDescriptionLink", DESCRIPTIONPATH, "" ) );
	return 1;
}

int CLuaTechnology::GetCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::GetCompany not need any parameter");

	CNrpCompany* cmp = NULL;
	IF_OBJECT_NOT_NULL_THEN cmp = (*_object)[ PARENTCOMPANY ].As<CNrpCompany*>();

	lua_pop( L, argc );
	lua_pushlightuserdata( L, cmp );
	Luna< CLuaCompany >::constructor( L );

	return 1;		
}

int CLuaTechnology::SetCompany( lua_State* L )
{
	return SetParam_<CNrpCompany*, void*>( L, "SetCompany", PARENTCOMPANY, lua_touserdata );
}

int CLuaTechnology::GetInternalName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetInternalName", INTERNAL_NAME, "" ) );
	return 1;
}

const char* CLuaTechnology::ClassName()
{
	return ( CLASS_LUATECH );
}
}//namespace nrp