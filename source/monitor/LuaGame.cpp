#include "StdAfx.h"
#include <assert.h>

#include "LuaGame.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"

using namespace irr;

namespace nrp
{

Luna< CLuaGame >::RegType CLuaGame::methods[] =			//реализуемы методы
{
	LUNA_ILUAOBJECT_HEADER( CLuaGame ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaGame, HaveBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetName ),
	{0,0}
};

CLuaGame::CLuaGame(lua_State *L) : ILuaObject(L, "CLuaGame" )							//конструктор
{}

int CLuaGame::HaveBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:HaveBox not need any parameter" );

	bool haveBox = false;
	IF_OBJECT_NOT_NULL_THEN	haveBox = object_->GetValue<PNrpGameBox>( GAMEBOX ) != NULL;

	lua_pushboolean( L, haveBox );
	return 1;	
}

int CLuaGame::GetName( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:HaveBox not need any parameter" );

	std::string name = "error";
	IF_OBJECT_NOT_NULL_THEN	name = object_->GetValue<std::string>( NAME );

	lua_pushstring( L, name.c_str() );
	return 1;	
}

int CLuaGame::IsMyBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:IsMyBoxAddon need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	bool isMyBox = false;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GAMEBOX );
		if( box != NULL )
			isMyBox = box->IsMyBoxAddon( name );

	}
	lua_pushboolean( L, isMyBox );
	return 1;	
}

int CLuaGame::RemoveBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:RemoveBoxAddon need string parameter" );

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GAMEBOX );
		if( box != NULL )
			box->RemoveMyBoxAddon( name );

	}
	return 1;	
}

int CLuaGame::AddBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:AddBoxAddon need CNrpTechnology parameter" );

	PNrpTechnology tech = (PNrpTechnology)lua_touserdata( L, 2 );
	assert( tech != NULL );
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GAMEBOX );
		assert( box != NULL );
		if( box != NULL )
			box->AddBoxAddon( tech );

	}
	return 1;	
}
}//namespace nrp