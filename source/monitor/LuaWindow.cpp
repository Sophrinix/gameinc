#include "StdAfx.h"
#include <IGUIWindow.h>

#include "LuaWindow.h"
#include "NrpWindow.h"
#include "NrpText.h"
#include "LuaButton.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAWINDOW( "CLuaWindow" );

BEGIN_LUNA_METHODS(CLuaWindow)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaWindow )
	/*                */
	LUNA_AUTONAME_FUNCTION( CLuaWindow, Bind )
	LUNA_AUTONAME_FUNCTION( CLuaWindow, Unbind )
	LUNA_AUTONAME_FUNCTION( CLuaWindow, Draw )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaWindow)
	LUNA_ILUAGUIELEMENT_PROPERTIES(CLuaWindow)
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "texture", GetTexture, SetTexture )
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "modal", PureFunction, SetModal )
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "closeButton", GetCloseButton, PureFunction )
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "draggable", PureFunction, SetDraggable )
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "drawBody", PureFunction, SetDrawBody )
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "onRemove", PureFunction, SetOnRemove )
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "onLmbClick", PureFunction, SetOnLmbClick )
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "onKeyEvent", PureFunction, SetOnKeyEvent )
END_LUNA_PROPERTIES

CLuaWindow::CLuaWindow(lua_State *L, bool exist) : ILuaGuiElement(L, CLASS_LUAWINDOW, exist )
{}

int CLuaWindow::GetCloseButton( lua_State *L )
{
	int argc = lua_gettop(L);
	IF_OBJECT_NOT_NULL_THEN
	{
		void* button = (void*)(_object->getCloseButton());
		//lua_pop( L, argc );
		lua_pushlightuserdata( L, button );
		Luna< CLuaButton >::constructor( L );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaWindow::SetOnRemove( lua_State* L )
{
	assert( lua_isfunction( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN _object->Bind( GUIELEMENT_ON_REMOVE, _GetRef( L, -1 ) );

	return 0;
}

int CLuaWindow::SetOnLmbClick( lua_State* L )
{
	assert( lua_isfunction( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN _object->Bind( GUIELEMENT_LMOUSE_LEFTUP, _GetRef( L, -1 ) );

	return 0;
}

int CLuaWindow::SetTexture( lua_State* L )
{
	DumpStack( L );
	IF_OBJECT_NOT_NULL_THEN	
	{
		if( lua_isstring( L, -1 ) )
		{
			NrpText txsPath = lua_tostring( L, -1 );
			video::ITexture* txs = _nrpEngine.GetVideoDriver()->getTexture( txsPath );
			_object->setBackgroundTexture( txs );
		}
	}

	return 0;
}

int CLuaWindow::GetTexture( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN	
	{
		video::ITexture* txs = _object->getBackgroundTexture();
		lua_pushstring( L, txs->getName().getPath().c_str() );
		return 1;
	}

	lua_pushnil( L );
	return 1;
}

int CLuaWindow::SetDraggable( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		bool drag = lua_toboolean( L, -1 ) > 0;
		_object->setDraggable( drag );
	}

	return 0;
}

int CLuaWindow::SetDrawBody( lua_State *L )
{
	IF_OBJECT_NOT_NULL_THEN
	{
		bool drag = lua_toboolean( L, -1 ) > 0;
		_object->setDrawBackground( drag );
	}

	return 0;
}

int CLuaWindow::Bind( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaWindow:AddLuaFunction need 2 parameter ");

	assert( lua_isfunction( L, 3 ) );
	IF_OBJECT_NOT_NULL_THEN	
	{
		int typef = lua_tointeger( L, 2 );
		int name = _GetRef( L, 3 );
		assert( typef );
		_object->Bind( typef, name );
	}

	return 0;
}

int CLuaWindow::Unbind( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaWindow:Unbind need 2 parameter ");

	IF_OBJECT_NOT_NULL_THEN	
	{
		int typef = lua_tointeger( L, 2 );
		int name = lua_tointeger( L, 3 );
		assert( typef && name );
		_object->Unbind( typef, name );
	}

	return 0;
}

int CLuaWindow::Draw( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaWindow::Draw not need any parameter");

	IF_OBJECT_NOT_NULL_THEN	_object->draw();

	return 0;
}

const char* CLuaWindow::ClassName()
{
	return CLASS_LUAWINDOW;
}

int CLuaWindow::SetOnKeyEvent( lua_State* L )
{
	assert( lua_isfunction( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN _object->Bind( GUIELEMENT_KEY_INPUT, _GetRef( L, -1 ) );

	return 0;
}

int CLuaWindow::SetModal( lua_State* L )
{
	assert( lua_isboolean( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		if( lua_toboolean( L, -1 ) != 0)
			_object->setModal();
	}

	return 0;
}

}//namespace nrp