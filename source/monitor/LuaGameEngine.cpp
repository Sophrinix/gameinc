#include "StdAfx.h"
#include "LuaGameEngine.h"
#include "NrpGameEngine.h"
#include "NrpTechnology.h"
#include "LuaTechnology.h"
#include "NrpApplication.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAGAMEENGINE( "CLuaGameEngine" );

Luna< CLuaGameEngine >::RegType CLuaGameEngine::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaGameEngine ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, AddGenre ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, SetGenreModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetGenreModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetAdvancedTechNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, SetCodeVolume ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, IsMyGenre ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, Load ),
	{0,0}
};

CLuaGameEngine::CLuaGameEngine(lua_State *L) : ILuaProject(L, "CLuaGameEngine")	//конструктор
{}

int CLuaGameEngine::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:Create need string parameter" );

	NrpText name = lua_tostring( L, 2 );

	_object = new CNrpGameEngine( name );
	_nrpApp.AddGameEngine( _object );

	lua_pushlightuserdata(L, _object );
	return 1;
}

int CLuaGameEngine::Load( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:Load need string parameter" );

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->Load( name );

	return 1;
}

int CLuaGameEngine::AddGenre( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:AddGenre need int parameter" );

	NrpText genre = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN  _object->AddGenre( genre );

	return 1;
}

int CLuaGameEngine::SetGenreModuleNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:SetGenreModuleNumber need int parameter" );

	int genreNumber = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN  (*_object)[ GENRE_MODULE_NUMBER ] = genreNumber;

	return 1;
}

int CLuaGameEngine::GetGenreModuleNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetGenreModuleNumber", GENRE_MODULE_NUMBER, 0 ) ) ;
	return 1;
}

int CLuaGameEngine::SetCodeVolume( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:SetCodeVolume need parameter" );

	int volCode = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN  (*_object)[ CODEVOLUME ] = volCode;

	return 1;
}

int CLuaGameEngine::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetName", NAME, "") );
	return 1;
}

int CLuaGameEngine::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<NrpText>( L, "GetTexture", TEXTURENORMAL, "") );
	return 1;
}

int CLuaGameEngine::GetAdvancedTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, "GetAdvancedTechNumber", ADVTECHNUMBER, 0 ) ) ;
	return 1;
}

const char* CLuaGameEngine::ClassName()
{
	return ( CLASS_LUAGAMEENGINE );
}

int CLuaGameEngine::IsMyGenre( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:IsMyGenre need CLuaTech parameter" );

	CNrpTechnology* genre = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2 );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN  ret = _object->IsGenreAvailble( (*genre)[ INTERNAL_NAME ] );

	lua_pushboolean( L, ret );
	return 1;
}
}//namespace nrp