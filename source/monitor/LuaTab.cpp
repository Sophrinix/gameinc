#include "StdAfx.h"
#include <IVideoDriver.h>
#include <IGUITabControl.h>
#include "nrpEngine.h"
#include "NrpText.h"
#include "nrpGUIEnvironment.h"
#include "LuaTab.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUATAB( "CLuaTab" );

BEGIN_LUNA_METHODS(CLuaTab)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaTab )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaTab, SetFontFromSize )
	LUNA_AUTONAME_FUNCTION( CLuaTab, SetTextColor )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaTab)
END_LUNA_PROPERTIES

CLuaTab::CLuaTab(lua_State *L, bool ex) : ILuaGuiElement(L, CLASS_LUATAB, ex)
{}

int CLuaTab::SetFontFromSize( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTab::SetFontFromSize need size parameter");

	int size = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		irr::gui::IGUIFont* font = _nrpEngine.GetGuiEnvironment()->getFont( NrpText("font_") + NrpText( size ) );
		_object->setRFont( font );
	}

	return 1;
}

int CLuaTab::SetTextColor( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaTab::SetTextColor need size parameter");

	irr::video::SColor color( lua_tointeger( L, 2 ), 
							  lua_tointeger( L, 3 ),
							  lua_tointeger( L, 4 ),
							  lua_tointeger( L, 5 ) );

	IF_OBJECT_NOT_NULL_THEN _object->setTextColor( color );

	return 1;
}

const char* CLuaTab::ClassName()
{
	return ( CLASS_LUATAB );
}
}//namespace nrp