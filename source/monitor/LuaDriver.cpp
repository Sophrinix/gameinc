#include "StdAfx.h"
#include "LuaDriver.h"

#include <IVideoDriver.h>
#include <assert.h>
#include "LuaTexture.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUADRIVER( "CLuaDriver" );

BEGIN_LUNA_METHODS(CLuaDriver)
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaDriver, GetScreenSize )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, GetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, AddTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, AddRenderTargetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, RemoveTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, SetRenderTarget )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaDriver)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaDriver )
END_LUNA_PROPERTIES

CLuaDriver::CLuaDriver(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUADRIVER, ex)
{}

int CLuaDriver::GetScreenSize( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDriver:GetScreenSize not need any parameter");

	core::dimension2di size(0, 0);

	IF_OBJECT_NOT_NULL_THEN size = _object->getScreenSize();
	lua_pushinteger( L, size.Width );
	lua_pushinteger( L, size.Height );

	return 2;
}

int CLuaDriver::GetTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDriver:GetTexture need string parameter");

	NrpText fileName = lua_tostring( L, 2 );

	video::ITexture* txs = NULL;
	IF_OBJECT_NOT_NULL_THEN txs = _object->getTexture( fileName );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, txs );
	Luna< CLuaTexture >::constructor( L );

	return 1;
}

int CLuaDriver::AddTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaDriver:AddTexture need 3 parameter");

	u32 width = (u32)lua_tointeger( L, 2 );
	u32 height = (u32)lua_tointeger( L, 3 );
	NrpText fileName = lua_tostring( L, 4 );

	video::ITexture* txs = NULL;
	IF_OBJECT_NOT_NULL_THEN txs = _object->addTexture( core::dimension2du( width, height ), fileName );
	lua_pushlightuserdata( L, txs );

	return 1;
}

int CLuaDriver::RemoveTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDriver:RemoveTexture need string parameter");

	NrpText fileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		video::ITexture* txs = _object->getTexture( fileName );
		_object->removeTexture( txs );
	}

	return 1;
}

int CLuaDriver::AddRenderTargetTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaDriver:AddRenderTargetTexture need 3 parameter");

	u32 width = (u32)lua_tointeger( L, 2 );
	u32 height = (u32)lua_tointeger( L, 3 );
	NrpText fileName = lua_tostring( L, 4 );

	video::ITexture* txs = NULL;
	IF_OBJECT_NOT_NULL_THEN txs = _object->addRenderTargetTexture( core::dimension2du( width, height ), fileName, video::ECF_A8R8G8B8 );

	lua_pop( L, argc );
	lua_pushlightuserdata( L, txs );
	Luna< CLuaTexture >::constructor( L );

	return 1;
}

int CLuaDriver::SetRenderTarget( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 8, 8, "Function CLuaDriver:SetRenderTarget need 3 parameter");

	video::ITexture* txs = (video::ITexture*)lua_touserdata( L, 2 );
	bool clearBackBuffer = lua_toboolean( L, 3 ) > 0;
	bool clearZBuffer = lua_toboolean( L, 4 ) > 0;
	video::SColor color( lua_tointeger( L, 5 ), lua_tointeger( L, 6 ), lua_tointeger( L, 7 ), lua_tointeger( L, 8) );

	IF_OBJECT_NOT_NULL_THEN _object->setRenderTarget( txs, clearBackBuffer, clearZBuffer, color );

	return 1;
}

const char* CLuaDriver::ClassName()
{
	return ( CLASS_LUADRIVER );
}
}