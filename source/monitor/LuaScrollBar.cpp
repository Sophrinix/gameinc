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

BEGIN_LUNA_METHODS(CLuaScrollBar)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaScrollBar )
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, GetUpButton )
	LUNA_AUTONAME_FUNCTION( CLuaScrollBar, GetDownButton )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaScrollBar)
	LUNA_ILUAGUIELEMENT_PROPERTIES(CLuaScrollBar)
	LUNA_AUTONAME_PROPERTY( CLuaScrollBar, "texture", PureFunction, SetTexture )
	LUNA_AUTONAME_PROPERTY( CLuaScrollBar, "sliderTexture", PureFunction, SetSliderTexture )
	LUNA_AUTONAME_PROPERTY( CLuaScrollBar, "onAction", PureFunction, SetAction )
	LUNA_AUTONAME_PROPERTY( CLuaScrollBar, "min", PureFunction, SetMin )
	LUNA_AUTONAME_PROPERTY( CLuaScrollBar, "max", PureFunction, SetMax )
	LUNA_AUTONAME_PROPERTY( CLuaScrollBar, "pos", GetPos, PureFunction )
END_LUNA_PROPERTIES

CLuaScrollBar::CLuaScrollBar(lua_State *L, bool ex) : ILuaGuiElement(L, CLASS_LUASCROLLBAR, ex)
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
	IF_OBJECT_NOT_NULL_THEN
	{
		int pos = _object->getPos();
		lua_pushinteger( L, pos );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaScrollBar::SetMin( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN 
	{
		int amount = lua_tointeger( L, -1 );
		_object->setMin( amount );
	}

	return 0;
}

int CLuaScrollBar::SetMax( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		int amount = lua_tointeger( L, -1 );
		_object->setMax( amount );
	}

	return 0;
}

int CLuaScrollBar::SetTexture( lua_State *L )
{
	assert( lua_isstring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN 
	{
		NrpText filename = lua_tostring( L, -1 );
		_object->setTexture( _nrpEngine.GetVideoDriver()->getTexture( filename ) );
	}

	return 0;
}

int CLuaScrollBar::SetAction( lua_State *L )
{
	assert( lua_isfunction( L, -1) );
	IF_OBJECT_NOT_NULL_THEN _object->setAction( _GetRef( L, -1 ) );
	return 0;
}

int CLuaScrollBar::SetSliderTexture( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN 
	{
		NrpText filename = lua_tostring( L, -1 );
		_object->setSliderTexture( _nrpEngine.GetVideoDriver()->getTexture( filename ) );
	}
	return 0;
}

const char* CLuaScrollBar::ClassName()
{
	return ( CLASS_LUASCROLLBAR );
}
}