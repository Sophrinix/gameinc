#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaProgressBar.h"
#include "NrpProgressBar.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAPROGRESBAR( "CLuaProgressBar" );

BEGIN_LUNA_METHODS(CLuaProgressBar)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaProgressBar )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaProgressBar)
	LUNA_ILUAGUIELEMENT_PROPERTIES(CLuaProgressBar )
	LUNA_AUTONAME_PROPERTY( CLuaProgressBar, "progress", PureFunction, SetPosition )
	LUNA_AUTONAME_PROPERTY( CLuaProgressBar, "image", PureFunction, SetImage )
	LUNA_AUTONAME_PROPERTY( CLuaProgressBar, "fillImage", PureFunction, SetFillImage )
END_LUNA_PROPERTIES

CLuaProgressBar::CLuaProgressBar(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAPROGRESBAR, ex )							//конструктор
{}

int CLuaProgressBar::SetPosition( lua_State* L )
{
	assert( lua_isnumber( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		int position = lua_tointeger( L, -1 );
		_object->setPosition( position );
	}

	return 1;
}

int CLuaProgressBar::SetImage( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText textureName = lua_tostring( L, -1 );

		video::ITexture* ptrTexture = _nrpEngine.GetVideoDriver()->getTexture( textureName );
		_object->setImage( ptrTexture );
	}

	return 1;	
}

int CLuaProgressBar::SetFillImage( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText textureName = lua_tostring( L, -1 );
		video::ITexture* ptrTexture = _nrpEngine.GetVideoDriver()->getTexture( textureName );
		_object->setFillImage( ptrTexture );
	}

	return 1;	
}

const char* CLuaProgressBar::ClassName()
{
	return ( CLASS_LUAPROGRESBAR );
}
}//namespace nrp