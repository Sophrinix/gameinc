#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaEdit.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAEDIT( "CLuaEdit" );

BEGIN_LUNA_METHODS(CLuaEdit)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaEdit )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaEdit)
	LUNA_ILUAGUIELEMENT_PROPERTIES(CLuaEdit)
	LUNA_AUTONAME_PROPERTY( CLuaEdit, "overrideColor", PureFunction, SetOverrideColor )
	LUNA_AUTONAME_PROPERTY( CLuaEdit, "isPasswordBox", PureFunction, SetPasswordBox )
	LUNA_AUTONAME_PROPERTY( CLuaEdit, "drawBody", PureFunction, SetDrawBorder )
	LUNA_AUTONAME_PROPERTY( CLuaEdit, "multiline", PureFunction, SetMultiline ) 
END_LUNA_PROPERTIES

CLuaEdit::CLuaEdit(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAEDIT, ex )							//конструктор
{}

int CLuaEdit::SetOverrideColor( lua_State* L )
{
	int ovColor = lua_tointeger( L, -1 );

	IF_OBJECT_NOT_NULL_THEN	_object->setOverrideColor( ovColor );

	return 0;
}

int CLuaEdit::SetDrawBorder( lua_State* L )
{
	bool draw = lua_toboolean( L, -1 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	_object->setDrawBorder( draw );

	return 0;
}

int CLuaEdit::SetMultiline( lua_State* L )
{
	bool isMl = lua_toboolean( L, -1 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	_object->setMultiLine( isMl );

	return 0;
}

int CLuaEdit::SetPasswordBox( lua_State* L )
{
	bool isPassBox = lua_toboolean( L, -1 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	_object->setPasswordBox( isPassBox );

	return 0;
}

const char* CLuaEdit::ClassName()
{
	return ( CLASS_LUAEDIT );
}
}//namespace nrp