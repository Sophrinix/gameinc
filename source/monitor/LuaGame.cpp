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
	LUNA_AUTONAME_FUNCTION( CLuaGame, IsMyBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, RemoveBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, AddBoxAddon ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxAddonsNumber ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, CreateBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, RemoveBox ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxLevel ),
	LUNA_AUTONAME_FUNCTION( CLuaGame, GetBoxAddon ),
	{0,0}
};

CLuaGame::CLuaGame(lua_State *L) : ILuaObject(L, "CLuaGame" )							//конструктор
{}

int CLuaGame::HaveBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:HaveBox not need any parameter" );

	bool haveBox = false;
	IF_OBJECT_NOT_NULL_THEN	haveBox = object_->GetValue<PNrpGameBox>( GBOX ) != NULL;

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
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
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
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
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
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		assert( box != NULL );
		if( box != NULL )
			box->AddBoxAddon( tech );

	}
	return 1;	
}

int CLuaGame::GetBoxAddonsNumber( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetGameBoxAddonsNumber not need any parameter" );

	int addonNumber = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box != NULL)
			addonNumber = box->GetValue<int>( NUMBERADDON );
	}

	lua_pushinteger( L, addonNumber );
	return 1;		
}

int CLuaGame::CreateBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:CreateBox need int parameter" );

	int boxLevel = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = new CNrpGameBox( object_ );
		box->SetValue<int>( LEVEL, boxLevel );
		object_->SetValue<PNrpGameBox>( GBOX, box );
	}
	return 1;		
}

int CLuaGame::RemoveBox( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:RemoveBox not need any parameter" );

	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		object_->SetValue<PNrpGameBox>( GBOX, NULL );
		delete box;
	}

	return 1;			
}

int CLuaGame::GetBoxLevel( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaGame:GetBoxLevel not need any parameter" );

	int boxLevel = 0;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box != NULL )
			boxLevel = box->GetValue<int>( LEVEL );
	}

	lua_pushinteger( L, boxLevel );
	return 1;			
}

int CLuaGame::GetBoxAddon( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaGame:GetBoxAddon need int parameter" );

	int index = lua_tointeger( L, 2 );
	CNrpTechnology* tech = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		PNrpGameBox box = object_->GetValue<PNrpGameBox>( GBOX );
		if( box )
			tech = box->GetAddon( index );
	}

	lua_pushlightuserdata( L, tech );
	return 1;		
}
}//namespace nrp