#include "StdAfx.h"
#include <IVideoDriver.h>
#include <string>
#include <IGUITabControl.h>
#include "nrpEngine.h"
#include "StrConversation.h"
#include "nrpGUIEnvironment.h"
#include "LuaTab.h"

using namespace irr;

namespace nrp
{

Luna< CLuaTab >::RegType CLuaTab::methods[] = 
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaTab ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaTab, SetFontFromSize ),
	LUNA_AUTONAME_FUNCTION( CLuaTab, SetTextColor ),
	{0,0}
};

CLuaTab::CLuaTab(lua_State *L) : ILuaGuiElement(L, "CLuaTab")
{}

int CLuaTab::SetFontFromSize( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaTab::SetFontFromSize need size parameter");

	int size = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		irr::gui::IGUIFont* font = CNrpEngine::Instance().GetGuiEnvironment()->getFont( ("font_" + conv::ToStr( size )).c_str() );
		object_->setRFont( font );
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

	IF_OBJECT_NOT_NULL_THEN object_->setTextColor( color );

	return 1;
}
}//namespace nrp