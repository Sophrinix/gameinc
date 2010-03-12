#include "StdAfx.h"
#include <IVideoDriver.h>
#include <string>
#include "LuaConsole.h"
#include "nrpConsole.h"
#include "nrpEngine.h"
#include "StrConversation.h"

using namespace irr;

namespace nrp
{

Luna< CLuaConsole >::RegType CLuaConsole::methods[] = 
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaConsole ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaConsole, ToggleVisible ),
	{0,0}
};

CLuaConsole::CLuaConsole(lua_State *L) : ILuaGuiElement(L, "CLuaConsole")
{}

int CLuaConsole::ToggleVisible( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaConsole::ToggleVisible not need any parameter");

	IF_OBJECT_NOT_NULL_THEN object_->ToggleVisible();

	return 1;
}
}//namespace nrp