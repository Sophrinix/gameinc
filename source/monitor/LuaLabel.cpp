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
	LUNA_AUTONAME_FUNCTION( CLuaLabel, SetTextAlignment )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaLabel)
	LUNA_ILUAGUIELEMENT_PROPERTIES(CLuaLabel)
	LUNA_AUTONAME_PROPERTY( CLuaLabel, "color", PureFunction, SetOverrideColor )
	LUNA_AUTONAME_PROPERTY( CLuaLabel, "font", PureFunction, SetOverrideFont )
END_LUNA_PROPERTIES

CLuaLabel::CLuaLabel(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUALABEL, ex )							//конструктор
{}

int CLuaLabel::SetOverrideFont( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText fontName = lua_tostring( L, -1 );
		gui::IGUIFont* font = _nrpEngine.GetGuiEnvironment()->getFont( fontName );
		_object->setOverrideFont( font );
	}

	return 0;
}

int CLuaLabel::SetOverrideColor( lua_State* L )
{
	int ovColor = lua_tointeger( L, -1 );
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