#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaLabel.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUALABEL( "CLuaLabel" );

BEGIN_LUNA_METHODS(CLuaLabel)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaLabel )
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaLabel, SetOverrideColor )
	LUNA_AUTONAME_FUNCTION( CLuaLabel, SetOverrideFont )
	LUNA_AUTONAME_FUNCTION( CLuaLabel, SetTextAlignment )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaLabel)
END_LUNA_PROPERTIES

CLuaLabel::CLuaLabel(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUALABEL, ex )							//конструктор
{}

int CLuaLabel::SetOverrideFont( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel::SetOverrideFont need fontName parameter");

	NrpText fontName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		gui::IGUIFont* font = _nrpEngine.GetGuiEnvironment()->getFont( fontName );
		_object->setOverrideFont( font );
	}

	return 1;
}

int CLuaLabel::SetOverrideColor( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 5, 5, "Function CLuaLabel::SetOverrideColor need 4 int parameter");

	int ovColor = (lua_tointeger( L, 2 ) << 24);
	ovColor += (lua_tointeger( L, 3 ) << 16);
	ovColor += (lua_tointeger( L, 4 ) << 8);
	ovColor += lua_tointeger( L, 5 );

	IF_OBJECT_NOT_NULL_THEN	_object->setOverrideColor( ovColor );

	return 1;
}

int CLuaLabel::SetTextAlignment( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaLabel::SetTextAlignment need bool, bool parameter");
	
	int hAl = lua_tointeger( L, 2 );
	int vAl = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->setTextAlignment( gui::EGUI_ALIGNMENT(hAl), gui::EGUI_ALIGNMENT(vAl) );

	return 1;
}

const char* CLuaLabel::ClassName()
{
	return ( CLASS_LUALABEL );
}
}//namespace nrp