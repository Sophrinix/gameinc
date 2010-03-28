#include "StdAfx.h"
#include "LuaTexture.h"

#include <ITexture.h>

using namespace irr;

namespace nrp
{

Luna< CLuaTexture >::RegType CLuaTexture::methods[] = 
{
	LUNA_ILUAOBJECT_HEADER( CLuaTexture ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaTexture, GetSize ),
	LUNA_AUTONAME_FUNCTION( CLuaTexture, GetWidth ),
	LUNA_AUTONAME_FUNCTION( CLuaTexture, GetHeight ),
	LUNA_AUTONAME_FUNCTION( CLuaTexture, Drop ),
	{0,0}
};

CLuaTexture::CLuaTexture(lua_State *L) : ILuaObject( L, "CLuaTexture" )
{}

int CLuaTexture::GetSize( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTexture:GetSize not need any parameter");

	core::dimension2di size;
	IF_OBJECT_NOT_NULL_THEN size = object_->getSize();

	lua_pushinteger( L, size.Width );
	lua_pushinteger( L, size.Height );

	return 2;
}

int CLuaTexture::GetWidth( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTexture::GetWidth not need any parameter");

	int width = 0;

	IF_OBJECT_NOT_NULL_THEN width = object_->getSize().Width;
	lua_pushinteger( L, width );

	return 1;
}

int CLuaTexture::GetHeight( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTexture:GetHeight not need any parameter");

	int height = 0;

	IF_OBJECT_NOT_NULL_THEN height = object_->getSize().Height;
	lua_pushinteger( L, height );

	return 1;
}

int CLuaTexture::Drop( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaTexture:Drop not need any parameter");

	int height = 0;

	IF_OBJECT_NOT_NULL_THEN object_->drop();

	return 1;
}
}
