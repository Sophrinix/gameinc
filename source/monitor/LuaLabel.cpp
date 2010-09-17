#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaLabel.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{

Luna< CLuaLabel >::RegType CLuaLabel::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaLabel ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaLabel, SetOverrideColor ),
	LUNA_AUTONAME_FUNCTION( CLuaLabel, SetTextAlignment ),
	{0,0}
};

CLuaLabel::CLuaLabel(lua_State *L)	: ILuaGuiElement(L, "CLuaLabel")							//конструктор
{}

int CLuaLabel::SetOverrideColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaLabel::SetOverrideColor need 4 int parameter");

	int ovColor = (lua_tointeger( L, 2 ) << 24);
	ovColor += (lua_tointeger( L, 3 ) << 16);
	ovColor += (lua_tointeger( L, 4 ) << 8);
	ovColor += lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN	object_->setOverrideColor( ovColor );

	return 1;
}

int CLuaLabel::SetTextAlignment( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaLabel::SetTextAlignment need bool, bool parameter");
	
	int hAl = lua_tointeger( L, 2 );
	int vAl = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	object_->setTextAlignment( gui::EGUI_ALIGNMENT(hAl), gui::EGUI_ALIGNMENT(vAl) );

	return 1;
}
}//namespace nrp