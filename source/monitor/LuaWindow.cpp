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

Luna< CLuaWindow >::RegType CLuaWindow::methods[] = 
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaWindow ),
	/*                */
	LUNA_AUTONAME_FUNCTION( CLuaWindow, GetCloseButton ),
	LUNA_AUTONAME_FUNCTION( CLuaWindow, SetDraggable ),
	LUNA_AUTONAME_FUNCTION( CLuaWindow, SetDrawBody ),
	LUNA_AUTONAME_FUNCTION( CLuaWindow, AddLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaWindow, RemoveLuaFunction ),
	LUNA_AUTONAME_FUNCTION( CLuaWindow, Draw ),
	{0,0}
};

CLuaWindow::CLuaWindow(lua_State *L) : ILuaGuiElement(L, CLASS_LUAWINDOW )
{}

int CLuaWindow::GetCloseButton( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaWindow::getCloseButton not need any parameter");
	void* button = NULL;

	IF_OBJECT_NOT_NULL_THEN	button = (void*)(object_->getCloseButton());

	lua_pop( L, argc );
	lua_pushlightuserdata( L, button );
	Luna< CLuaButton >::constructor( L );

	return 1;
}

int CLuaWindow::SetDraggable( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaWindow::setDraggable need bool parameter");

	bool drag = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN object_->setDraggable( drag );

	return 1;
}

int CLuaWindow::SetDrawBody( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaWindow::SetDrawBody need bool parameter");

	bool drag = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	object_->setDrawBackground( drag );

	return 1;
}

int CLuaWindow::AddLuaFunction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaWindow:AddLuaFunction need 2 parameter ");

	int typef = lua_tointeger( L, 2 );
	NrpText name = lua_tostring( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	object_->AddLuaFunction( typef, name );

	return 1;
}

int CLuaWindow::RemoveLuaFunction( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaWindow:RemoveLuaFunction need 2 parameter ");

	int typef = lua_tointeger( L, 2 );
	NrpText name = lua_tostring( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	object_->RemoveLuaFunction( typef, name );

	return 1;
}

int CLuaWindow::Draw( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaWindow::Draw not need any parameter");

	IF_OBJECT_NOT_NULL_THEN	object_->draw();

	return 1;
}

const char* CLuaWindow::ClassName()
{
	return CLASS_LUAWINDOW;
}
}//namespace nrp