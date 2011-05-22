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
	LUNA_AUTONAME_PROPERTY( CLuaLabel, "drawBody", PureFunction, SetDrawBackground )
	LUNA_AUTONAME_PROPERTY( CLuaLabel, "wordWrap", PureFunction, SetWordWrap )
	LUNA_AUTONAME_PROPERTY( CLuaLabel, "image", PureFunction, SetImage )
END_LUNA_PROPERTIES

CLuaLabel::CLuaLabel(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUALABEL, ex )							//конструктор
{}

int CLuaLabel::SetFont( lua_State* L )
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

int CLuaLabel::SetWordWrap( lua_State* L )
{
	assert( lua_isboolean( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN _object->setWordWrap( lua_toboolean( L, -1 ) != 0 );

	return 0;
}

int CLuaLabel::SetDrawBackground( lua_State* L )
{
	assert( lua_isboolean( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		bool draw = lua_toboolean( L, -1 ) != 0;
		_object->setDrawBackground( draw );
	}

	return 0;
}

int CLuaLabel::SetOverrideColor( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN	
	{
		unsigned int color = 0;
		if( lua_isnumber( L, -1 ) )
			color = lua_tointeger( L, -1 );
		else if( lua_isstring( L, -1 ) )
		{
			NrpText ovColor = lua_tostring( L, -1 );
			color = ovColor.ToInt();
		}
		
			 
		_object->setOverrideColor( color );
	}

	return 0;
}

int CLuaLabel::SetTextAlignment( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 3, 3, "Function CLuaLabel::SetTextAlignment need bool, bool parameter");
	
	int hAl = lua_tointeger( L, 2 );
	int vAl = lua_tointeger( L, 3 );

	IF_OBJECT_NOT_NULL_THEN	_object->setTextAlignment( gui::EGUI_ALIGNMENT(hAl), gui::EGUI_ALIGNMENT(vAl) );

	return 0;
}

const char* CLuaLabel::ClassName()
{
	return ( CLASS_LUALABEL );
}

int CLuaLabel::SetImage( lua_State* L )
{
	return 0;
}

}//namespace nrp