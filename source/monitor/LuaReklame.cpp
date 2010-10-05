#include "StdAfx.h"
#include <irrlicht.h>
#include <assert.h>

#include "LuaReklame.h"
#include "NrpReklameWork.h"
#include "NrpPlant.h"

namespace nrp
{

Luna< CLuaReklame >::RegType CLuaReklame::methods[] =			//реализуемы методы
{
	LUNA_ILUAPROJECT_HEADER( CLuaReklame ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetQuality ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, Remove ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetTypeName ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetNumberDay ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, SetNumberDay ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetPrice ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetDayCost ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, GetFamous ),
	LUNA_AUTONAME_FUNCTION( CLuaReklame, SetReklameObject ),
	{0,0}
};

CLuaReklame::CLuaReklame(lua_State *L) : ILuaProject( L, CLASS_LUAREKLAME )							//конструктор
{}

int CLuaReklame::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaReklame:CreateTechnology need string, string parameter" );

	const char* typeName = lua_tostring( L, 2 );
	const char* gameName = lua_tostring( L, 3 );
	const char* company = lua_tostring( L, 4 );
	assert( typeName && gameName && company );

	object_ = CNrpPlant::Instance().CreateReklame( typeName, gameName, company );

	lua_pop( L, argc );
	lua_pushlightuserdata(L, object_ );
	Luna< CLuaReklame >::constructor( L );

	return 1;
}

int CLuaReklame::Remove( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaReklame::Remove not need parameter");

	IF_OBJECT_NOT_NULL_THEN	
	{
		delete object_;
		object_ = NULL;
	}

	return 1;	
}

int CLuaReklame::GetQuality( lua_State* L ) { lua_pushinteger( L, GetParam_<int>( L, "GetQuality", QUALITY, 0 ) ); return 1; }
int CLuaReklame::GetLevel( lua_State* L ) {	lua_pushinteger( L, GetParam_<int>( L, "GetLevel", LEVEL, 0 ) ); return 1; }
int CLuaReklame::GetNumberDay( lua_State* L ) { lua_pushinteger( L, GetParam_<int>( L, "GetNumberDay", NUMBERDAY, 0 ) ); return 1; }
int CLuaReklame::GetTypeName( lua_State* L ) { lua_pushstring( L, GetParam_<std::string>( L, "GetTypeName", TECHTYPE, "" ).c_str() ); return 1; }
int CLuaReklame::SetNumberDay( lua_State* L ) {	SetParam_<int, lua_Integer>( L, "SetNumberDay", NUMBERDAY, lua_tointeger );	return 1; }
int CLuaReklame::GetTexture( lua_State* L ) { lua_pushstring( L, GetParam_<std::string>( L, "GetTexture", TEXTURENORMAL, "" ).c_str() ); return 1; }
int CLuaReklame::GetPrice( lua_State* L ) {	lua_pushinteger( L, GetParam_<int>( L, "GetPrice", BALANCE, 0 ) ); return 1; }
int CLuaReklame::GetDayCost( lua_State* L ) { lua_pushinteger( L, GetParam_<int>( L, "GetDayCost", DAYCOST, 0 ) );return 1; }
int CLuaReklame::GetFamous( lua_State* L ) { lua_pushinteger( L, static_cast< int >( GetParam_<float>( L, "GetDayCost", MAXQUALITY, 0 ) * 100 ) ); return 1; }
int CLuaReklame::SetReklameObject( lua_State* L ) {	SetParam_( L, "SetReklameObject", GAMENAME ); return 1; }
}//namespace nrp