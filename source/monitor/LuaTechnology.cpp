#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaTechnology.h"
#include "NrpTechnology.h"

namespace nrp
{

Luna< CLuaTechnology >::RegType CLuaTechnology::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaTechnology ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetTechType ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetBaseCode ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetName ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetAddingEngineCode ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEngineTechRequire ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetEmployerSkillRequire ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTechType ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetTechGroup ),
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, GetOptionAsInt ),
	{0,0}
};

CLuaTechnology::CLuaTechnology(lua_State *L) : ILuaProject( L, "CLuaTech" )							//конструктор
{}

int CLuaTechnology::SetTechType( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::SetTechType need int parameter");

	int valuel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetOption<int>( TECHTYPE, valuel );

	return 1;	
}

int CLuaTechnology::GetTechType( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetTechType", TECHTYPE, 0 ) );
	return 1;	
}

int CLuaTechnology::SetBaseCode( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::SetBaseCode need int parameter");

	float valuel = (float)lua_tonumber( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetOption<float>( BASE_CODE, valuel );

	return 1;	
}

int CLuaTechnology::SetName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::SetName need string parameter");

	const char* valuel = lua_tostring( L, 2 );
	assert( valuel != NULL );

	IF_OBJECT_NOT_NULL_THEN	object_->SetOption<std::string>( NAME, std::string( valuel ) );

	return 1;	
}

int CLuaTechnology::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetName", NAME, "" ).c_str() );
	return 1;	
}

int CLuaTechnology::SetAddingEngineCode( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::SetAddingEngineCode need int parameter");

	float valuel = (float)lua_tonumber( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	object_->SetOption<float>( ENGINE_CODE, valuel );

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

int CLuaTechnology::GetTechGroup( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetTechGroup", TECHGROUP, 0 ) );
	return 1;	
}

int CLuaTechnology::GetOptionAsInt( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTechnology::GetOptionAsInt need int parameter");

	const char* opName = lua_tostring( L, 2 );
	assert( opName != NULL );

	int result = 0;
	IF_OBJECT_NOT_NULL_THEN	result = object_->GetOption<int>( opName );
	return 1;	
}
}//namespace nrp