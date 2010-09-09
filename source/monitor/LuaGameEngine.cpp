#include "StdAfx.h"
#include "LuaGameEngine.h"
#include "NrpGameEngine.h"
#include "NrpApplication.h"

using namespace irr;

namespace nrp
{

Luna< CLuaGameEngine >::RegType CLuaGameEngine::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaGameEngine ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, AddGenre ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, SetGenreModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetGenreModuleNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, SetCodeVolume ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetName ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, Create ),
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, GetTexture ),
	{0,0}
};

CLuaGameEngine::CLuaGameEngine(lua_State *L) : ILuaProject(L, "CLuaGameEngine")	//конструктор
{}

int CLuaGameEngine::Create( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:Create need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );

	object_ = new CNrpGameEngine( name );
	CNrpApplication::Instance().AddGameEngine( object_ );

	lua_pushlightuserdata(L, object_ );
	return 1;
}

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

	IF_OBJECT_NOT_NULL_THEN  object_->SetValue<int>( GENRE_MODULE_NUMBER, genreNumber );

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

	IF_OBJECT_NOT_NULL_THEN  object_->SetValue<int>( CODEVOLUME, volCode );

	return 1;
}

int CLuaGameEngine::GetName( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetName", NAME, "").c_str() );
	return 1;
}

int CLuaGameEngine::GetTexture( lua_State* L )
{
	lua_pushstring( L, GetParam_<std::string>( L, "GetTexture", TEXTURENORMAL, "").c_str() );
	return 1;
}

}//namespace nrp