#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaTechnology.h"
#include "NrpTechnology.h"

#define NO_POSTFIX
#define NO_ASSERT
#define GETTER_FUNCTION( name, lua_pushfunc, typen, paramName, defValue, postFix )\
	int CLuaTechnology::name( lua_State* L ) { lua_pushfunc( L, GetParam_<typen>( L, #name, paramName, defValue )postFix ); return 1; }

#define SETTER_FUNCTION( name, READTYPE, lua_tofunc, assertcode, OPTIONTYPE, paramName )\
	int CLuaTechnology::name( lua_State* L )\
	{	int argc = lua_gettop( L );\
		std::string funcName = #name;\
		luaL_argcheck( L, argc == 2, 2, ("Function CLuaTechnology:" + funcName + "need int parameter").c_str() );\
		READTYPE valuel = (READTYPE)lua_tofunc( L, 2 );\
		assertcode;\
		IF_OBJECT_NOT_NULL_THEN	object_->SetValue<OPTIONTYPE>( paramName, OPTIONTYPE(valuel) );\
		return 1;\
	}

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
	LUNA_AUTONAME_FUNCTION( CLuaTechnology, SetQuality ),
	{0,0}
};

CLuaTechnology::CLuaTechnology(lua_State *L) : ILuaProject( L, "CLuaTech" )							//конструктор
{}

GETTER_FUNCTION( GetTechType, lua_pushinteger, int, TECHTYPE, 0, NO_POSTFIX )
GETTER_FUNCTION( GetName, lua_pushstring, std::string, NAME, "", .c_str() )
GETTER_FUNCTION( GetTechGroup, lua_pushinteger, int, TECHGROUP, 0, NO_POSTFIX  )

SETTER_FUNCTION( SetBaseCode, float, lua_tonumber, NO_ASSERT, float, BASE_CODE )
SETTER_FUNCTION( SetName, const char*, lua_tostring, assert( valuel != NULL ), std::string, NAME )
SETTER_FUNCTION( SetTechType, int, lua_tointeger, NO_ASSERT, int, TECHTYPE )
SETTER_FUNCTION( SetAddingEngineCode, float, lua_tonumber, NO_ASSERT, float, ENGINE_CODE )
SETTER_FUNCTION( SetQuality, int, lua_tonumber, NO_ASSERT, int, QUALITY )

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

}//namespace nrp