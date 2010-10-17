#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaTechnology.h"
#include "NrpTechnology.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "LuaCompany.h"

#define NO_POSTFIX
#define NO_ASSERT

namespace nrp
{

Luna< CLuaTechnology >::RegType CLuaTechnology::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaTechnology ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetTechType ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetBaseCode ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetAddingEngineCode ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEngineTechRequire ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEmployerSkillRequire ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTechGroup ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetOptionAsInt ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, HaveLider ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Load ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, IsLoaded ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, HaveRequireTech ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetFutureTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetFutureTechInternalName ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetStatus ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetStatus ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetDescriptionLink ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetCompany ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetInternalName ),
	{0,0}
};

CLuaTechnology::CLuaTechnology(lua_State *L) : ILuaProject( L, CLASS_LUATECH )							//конструктор
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

int CLuaTechnology::SetTechType( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, "SetTechType", TECHTYPE, lua_tointeger );
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

	const char* opName = lua_tostring( L, 2 );
	assert( opName != NULL );

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetValue<int>( opName );
	lua_pushinteger( L, result );
	return 1;	
}

int CLuaTechnology::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaApplication:CreateTechnology need int parameter" );

	int typen = lua_tointeger( L, 2 );

	object_ = new CNrpTechnology( PROJECT_TYPE(typen) );
	lua_pushlightuserdata(L, object_ );

	return 1;
}

int CLuaTechnology::SetEngineTechRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEngineTechRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetEngineTechRequire( typen, valuel );

	return 1;	
}

int CLuaTechnology::SetEmployerSkillRequire( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaTechnology::SetEmployerSkillRequire need int,int parameter");

	int typen = lua_tointeger( L, 2 );
	int valuel = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetEmployerSkillRequire( typen, valuel );

	return 1;	
}

int CLuaTechnology::HaveLider( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::HaveLider not need parameter");

	bool haveUser = false; 
	IF_OBJECT_NOT_NULL_THEN	haveUser = !object_->GetValue<std::string>( COMPONENTLIDER ).empty();
	lua_pushboolean( L, haveUser );
	return 1;	
}

int CLuaTechnology::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology:Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete object_;
		object_ = NULL;
	}

	return 1;	
}

int CLuaTechnology::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::Load not need parameter");

	const char* iniFile = lua_tostring( L, 2 );
	assert( iniFile != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->Load( iniFile );
	return 1;	
}

int CLuaTechnology::GetLevel( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetLevel", LEVEL, 0 ) );
	return 1;
}

int CLuaTechnology::IsLoaded( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::IsLoaded not need any parameter");

	bool loaded = false; 
	IF_OBJECT_NOT_NULL_THEN loaded = CNrpApplication::Instance().GetBoxAddon( object_->GetValue<std::string>( NAME ) ) != NULL;

	lua_pushboolean( L, loaded );
	return 1;			
}

int CLuaTechnology::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetTexture", TEXTURENORMAL, "" ).c_str() );
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
	std::string name = "";
	IF_OBJECT_NOT_NULL_THEN
	{
		name = object_->GetFutureTech( index );
	}

	lua_pushstring( L, name.c_str() );
	return 1;	
}

int CLuaTechnology::SetStatus( lua_State* L )
{
	return SetParam_<int, lua_Integer>( L, "SetStatus", STATUS, lua_tointeger );
}

int CLuaTechnology::GetStatus( lua_State* L )
{
	lua_pushinteger( L, GetParam_<TECH_STATUS>( L, "GetStatus", STATUS, TS_UNKNOWN) );
	return 1;
}

int CLuaTechnology::GetDescriptionLink( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetDescriptionLink", DESCRIPTIONPATH, "" ).c_str() );
	return 1;
}

int CLuaTechnology::GetCompany( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTechnology::GetCompany not need any parameter");

	CNrpCompany* cmp = NULL;
	IF_OBJECT_NOT_NULL_THEN cmp = object_->GetValue<CNrpCompany*>( PARENTCOMPANY );

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
	lua_pushstring( L, GetParam_<std::string>( L, "GetInternalName", INTERNAL_NAME, "" ).c_str() );
	return 1;
}

}//namespace nrp