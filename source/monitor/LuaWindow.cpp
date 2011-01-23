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
	LUNA_AUTONAME_FUNCTION( CLuaWindow, GetCloseButton )
	LUNA_AUTONAME_FUNCTION( CLuaWindow, SetDraggable )
	LUNA_AUTONAME_FUNCTION( CLuaWindow, SetDrawBody )
	LUNA_AUTONAME_FUNCTION( CLuaWindow, AddLuaFunction )
	LUNA_AUTONAME_FUNCTION( CLuaWindow, RemoveLuaFunction )
	LUNA_AUTONAME_FUNCTION( CLuaWindow, Draw )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaWindow)
	LUNA_AUTONAME_PROPERTY( CLuaWindow, "texture", GetTexture, SetTexture )
END_LUNA_PROPERTIES

CLuaWindow::CLuaWindow(lua_State *L, bool exist) : ILuaGuiElement(L, CLASS_LUAWINDOW, exist )
{}

int CLuaWindow::GetCloseButton( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaWindow::getCloseButton not need any parameter");
	void* button = NULL;

	IF_OBJECT_NOT_NULL_THEN	button = (void*)(_object->getCloseButton());

	lua_pop( L, argc );
	lua_pushlightuserdata( L, button );
	Luna< CLuaButton >::constructor( L );

	return 1;
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
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaWindow::setDraggable need bool parameter");

	bool drag = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN _object->setDraggable( drag );

	return 1;
}

int CLuaWindow::SetDrawBody( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaWindow::SetDrawBody need bool parameter");

	bool drag = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	_object->setDrawBackground( drag );

	return 1;
}

int CLuaWindow::AddLuaFunction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaWindow:AddLuaFunction need 2 parameter ");

	int typef = lua_tointeger( L, 2 );
	NrpText name = lua_tostring( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->AddLuaFunction( typef, name );

	return 1;
}

int CLuaWindow::RemoveLuaFunction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaWindow:RemoveLuaFunction need 2 parameter ");

	int typef = lua_tointeger( L, 2 );
	NrpText name = lua_tostring( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->RemoveLuaFunction( typef, name );

	return 1;
}

int CLuaWindow::Draw( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaWindow::Draw not need any parameter");

	IF_OBJECT_NOT_NULL_THEN	_object->draw();

	return 1;
}

const char* CLuaWindow::ClassName()
{
	return CLASS_LUAWINDOW;
}
}//namespace nrp