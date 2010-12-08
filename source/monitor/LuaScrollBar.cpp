#include "StdAfx.h"
#include <IVideoDriver.h>
#include "LuaScrollBar.h"
#include "NrpMiniMap.h"
#include "nrpEngine.h"
#include "NrpText.h"
#include "NrpScrollBar.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUASCROLLBAR( "CLuaScrollBar" );

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

	IF_OBJECT_NOT_NULL_THEN btn = (void*)_object->getUpButton();
	lua_pushlightuserdata( L, btn );

	return 1;
}

int CLuaScrollBar::GetDownButton( lua_State* L  )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaMiniMap:GetSize not need any parameter");

	void* btn = NULL;

	IF_OBJECT_NOT_NULL_THEN btn = (void*)_object->getDownButton();
	lua_pushlightuserdata( L, btn );

	return 1;
}


int CLuaScrollBar::GetPos(  lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaMiniMap:GetSize not need any parameter");

	int pos = 0;

	IF_OBJECT_NOT_NULL_THEN pos = _object->getPos();
	lua_pushinteger( L, pos );

	return 1;
}

int CLuaScrollBar::SetMin( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetMin need 1 parameter");

	int amount = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->setMin( amount );

	return 1;
}

int CLuaScrollBar::SetMax( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetMax need 1 parameter");

	int amount = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->setMax( amount );

	return 1;
}

int CLuaScrollBar::SetTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetTexture need 1 parameter");

	NrpText filename = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->setTexture( _nrpEngine.GetVideoDriver()->getTexture( filename ) );

	return 1;
}

int CLuaScrollBar::SetAction( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::setName need 1 parameter");

	NrpText name = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->setAction( name );

	return 1;
}

int CLuaScrollBar::SetSliderTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaMiniMap::SetTexture need 1 parameter");

	NrpText filename = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN _object->setSliderTexture( _nrpEngine.GetVideoDriver()->getTexture( filename ) );

	return 1;
}

const char* CLuaScrollBar::ClassName()
{
	return ( CLASS_LUASCROLLBAR );
}
}