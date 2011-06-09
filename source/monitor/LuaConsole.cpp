#include "StdAfx.h"
#include <IVideoDriver.h>
#include "LuaConsole.h"
#include "nrpConsole.h"
#include "nrpEngine.h"
#include "NrpText.h"
#include "IC_Console_LuaCommand.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUACONSOLE( "CLuaConsole" );

BEGIN_LUNA_METHODS(CLuaConsole)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaConsole )
	LUNA_AUTONAME_FUNCTION( CLuaConsole, ToggleVisible )
	LUNA_AUTONAME_FUNCTION( CLuaConsole, Draw )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaConsole)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaConsole )
END_LUNA_PROPERTIES

CLuaConsole::CLuaConsole(lua_State *L, bool ex) : ILuaGuiElement(L, CLASS_LUACONSOLE, ex)
{}

int CLuaConsole::ToggleVisible( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaConsole::ToggleVisible not need any parameter");

	IF_OBJECT_NOT_NULL_THEN
	{
		if( gui::IGUIElement* ptr = _object->getParent() )
			ptr->bringToFront( _object );
		_object->ToggleVisible();
	}

	return 1;
}

int CLuaConsole::Draw( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaConsole::Draw not need any parameter");

	IF_OBJECT_NOT_NULL_THEN _object->draw();

	return 1;
}

const char* CLuaConsole::ClassName()
{
	return ( CLASS_LUACONSOLE );
}

int CLuaConsole::Bind( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaConsole::Bind need name, description, funcRef parameter");

	NrpText name = lua_tostring( L, 2 );
	NrpText desc = lua_tostring( L, 3 );
	int funcRef = _GetRef( L, 4 );

	IF_OBJECT_NOT_NULL_THEN
	{
		_object->RegisterCommand( new core::IC_Console_LuaCommand( name, desc, funcRef ) );
	}

	return 0;
}

int CLuaConsole::Unbind( lua_State* L )
{

	return 0;
}

}//namespace nrp