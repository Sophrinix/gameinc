#include "StdAfx.h"
#include <IVideoDriver.h>
#include <string>
#include "LuaScrollBar.h"
#include "NrpMiniMap.h"
#include "nrpEngine.h"
#include "StrConversation.h"
#include "NrpScrollBar.h"

using namespace irr;

namespace nrp
{

Luna< CLuaScrollBar >::RegType CLuaScrollBar::methods[] = 
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaScrollBar ),
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, SetTexture ),
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, SetAction ),
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, SetMin ),
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, SetMax ),
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, GetPos ),
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, GetUpButton ),
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, GetDownButton ),
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, SetSliderTexture ), 
	{0,0}
};

CLuaScrollBar::CLuaScrollBar(lua_State *L) : ILuaGuiElement(L, "CLuaScrollBar")
{}

int CLuaScrollBar::GetUpButton( lua_State* L  )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaMiniMap:GetSize not need any parameter");

	void* btn = NULL;

	IF_OBJECT_NOT_NULL_THEN btn = (void*)object_->getUpButton();
	lua_pushlightuserdata( L, btn );

	return 1;
}

int CLuaScrollBar::GetDownButton( lua_State* L  )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaMiniMap:GetSize not need any parameter");

	void* btn = NULL;

	IF_OBJECT_NOT_NULL_THEN btn = (void*)object_->getDownButton();
	lua_pushlightuserdata( L, btn );

	return 1;
}


int CLuaScrollBar::GetPos(  lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaMiniMap:GetSize not need any parameter");

	int pos = 0;

	IF_OBJECT_NOT_NULL_THEN pos = object_->getPos();
	lua_pushinteger( L, pos );

	return 1;
}

int CLuaScrollBar::SetMin( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetMin need 1 parameter");

	int amount = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->setMin( amount );

	return 1;
}

int CLuaScrollBar::SetMax( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetMax need 1 parameter");

	int amount = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN object_->setMax( amount );

	return 1;
}

int CLuaScrollBar::SetTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetTexture need 1 parameter");

	const char* filename = lua_tostring( L, 2 );
	assert( filename != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->setTexture( CNrpEngine::Instance().GetVideoDriver()->getTexture( filename ) );

	return 1;
}

int CLuaScrollBar::SetAction( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::setName need 1 parameter");

	const char* name = lua_tostring( L, 2 );
	assert( name != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->setAction( name );

	return 1;
}

int CLuaScrollBar::SetSliderTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetTexture need 1 parameter");

	const char* filename = lua_tostring( L, 2 );
	assert( filename != NULL );

	IF_OBJECT_NOT_NULL_THEN object_->setSliderTexture( CNrpEngine::Instance().GetVideoDriver()->getTexture( filename ) );

	return 1;
}

}