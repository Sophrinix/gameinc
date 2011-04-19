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

BEGIN_LUNA_METHODS(CLuaGameEngine)
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, AddGenre )
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, IsMyTech )
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, Create )
	LUNA_AUTONAME_FUNCTION( CLuaGameEngine, Load )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaGameEngine)
	LUNA_ILUAPROJECT_PROPERTIES( CLuaGameEngine )	
	LUNA_AUTONAME_PROPERTY(CLuaGameEngine, "codeVolume", PureFunction, SetCodeVolume )
	LUNA_AUTONAME_PROPERTY(CLuaGameEngine, "techNumber", GetTechNumber, PureFunction )
	LUNA_AUTONAME_PROPERTY(CLuaGameEngine, "genresNumber", GetGenreModuleNumber, SetGenreModuleNumber )
END_LUNA_PROPERTIES

CLuaGameEngine::CLuaGameEngine(lua_State *L, bool ex) : ILuaProject(L, CLASS_LUAGAMEENGINE, ex)	//конструктор
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
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	(*_object)[ GENRE_MODULE_NUMBER ] = lua_tointeger( L, 2 );
	return 0;
}

int CLuaGameEngine::GetGenreModuleNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, GENRE_MODULE_NUMBER, 0 ) ) ;
	return 1;
}

int CLuaGameEngine::SetCodeVolume( lua_State *L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN  (*_object)[ CODEVOLUME ] = lua_tointeger( L, -1 );

	return 1;
}

int CLuaGameEngine::GetTechNumber( lua_State* L )
{
	lua_pushinteger( L, GetParam_<int>( L, PROP, ADVTECHNUMBER, 0 ) ) ;
	return 1;
}

const char* CLuaGameEngine::ClassName()
{
	return ( CLASS_LUAGAMEENGINE );
}

int CLuaGameEngine::IsMyTech( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGameEngine:IsMyGenre need CLuaTech parameter" );

	CNrpTechnology* genre = _GetLuaObject< CNrpTechnology, CLuaTechnology >( L, 2 );

	bool ret = false;
	IF_OBJECT_NOT_NULL_THEN  ret = _object->IsMyTech( (*genre)[ INTERNAL_NAME ] );

	lua_pushboolean( L, ret );
	return 1;
}
}//namespace nrp