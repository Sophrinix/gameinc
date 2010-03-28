#include "StdAfx.h"
#include "LuaGameEngine.h"
#include "NrpGameEngine.h"

using namespace irr;

namespace nrp
{

Luna< CLuaGameEngine >::RegType CLuaGameEngine::methods[] =			//���������� ������
{
	LUNA_ILUAOBJECT_HEADER( CLuaGameEngine ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, AddGenre ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, SetGenreModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetGenreModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, SetCodeVolume ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetName ),
	{0,0}
};

CLuaGameEngine::CLuaGameEngine(lua_State *L) : ILuaProject(L, "CLuaGameEngine")	//�����������
{}

int CLuaGameEngine::AddGenre( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:AddGenre need int parameter" );

	int genre = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN  object_->AddGenre( GENRE_TYPE(genre) );

	return 1;
}

int CLuaGameEngine::SetGenreModuleNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:SetGenreModuleNumber need int parameter" );

	int genreNumber = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN  object_->SetOption<int>( GENRE_MODULE_NUMBER, genreNumber );

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

	IF_OBJECT_NOT_NULL_THEN  object_->SetOption<int>( CODEVOLUME, volCode );

	return 1;
}

int CLuaGameEngine::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetName", NAME, "").c_str() );
	return 1;
}
}//namespace nrp