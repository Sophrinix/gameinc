#include "StdAfx.h"
#include <IVideoDriver.h>
#include "LuaConsole.h"
#include "nrpConsole.h"
#include "nrpEngine.h"
#include "NrpText.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUACONSOLE( "CLuaConsole" );

Luna< CLuaConsole >::RegType CLuaConsole::methods[] = 
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaConsole ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaConsole, ToggleVisible ),
	LUNA_AUTONAME_FUNCTION( CLuaConsole, Draw ),
	{0,0}
};

CLuaConsole::CLuaConsole(lua_State *L) : ILuaGuiElement(L, "CLuaConsole")
{}

int CLuaConsole::ToggleVisible( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaConsole::ToggleVisible not need any parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		if( gui::IGUIElement* ptr = object_->getParent() )
			ptr->bringToFront( object_ );
		object_->ToggleVisible();
	}

	return 1;
}

int CLuaConsole::Draw( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaConsole::Draw not need any parameter");

	IF_OBJECT_NOT_NULL_THEN object_->draw();

	return 1;
}

const char* CLuaConsole::ClassName()
{
	return ( CLASS_LUACONSOLE );
}
}//namespace nrp