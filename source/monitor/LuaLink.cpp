#include "StdAfx.h"
#include "LuaLink.h"
#include "NrpGuiLink.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUALINK( "CLuaGuiLink" );

BEGIN_LUNA_METHODS(CLuaGuiLink)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaGuiLink )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaGuiLink)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaGuiLink )
	LUNA_AUTONAME_PROPERTY( CLuaGuiLink, "action", PureFunction, SetAction )
	LUNA_AUTONAME_PROPERTY( CLuaGuiLink, "tooltip", PureFunction, SetTooltip )
	LUNA_AUTONAME_PROPERTY( CLuaGuiLink, "color", PureFunction, SetOverrideColor )
END_LUNA_PROPERTIES


const char* CLuaGuiLink::ClassName()
{
	return 	( CLASS_LUALINK );
}

CLuaGuiLink::CLuaGuiLink(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUALINK, ex )							//конструктор
{}

int CLuaGuiLink::SetAction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	IF_OBJECT_NOT_NULL_THEN _object->setOnClickAction( _GetRef( L, -1 ) );

	return 0;
}

int CLuaGuiLink::SetTooltip( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	NrpText text( lua_tostring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	_object->setToolTipText( text.ToWide() );
	return 0;
}

int CLuaGuiLink::SetOverrideColor( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	int color = lua_tointeger( L, -1 );
	IF_OBJECT_NOT_NULL_THEN	_object->setOverrideColor( video::SColor( color ) );
	return 0;
}

}//namespace nrp