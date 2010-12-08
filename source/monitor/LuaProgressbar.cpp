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

Luna< CLuaProgressBar >::RegType CLuaProgressBar::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaProgressBar ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaProgressBar, SetPosition ),
	LUNA_AUTONAME_FUNCTION( CLuaProgressBar, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaProgressBar, SetFillImage ),
	{0,0}
};

CLuaProgressBar::CLuaProgressBar(lua_State *L)	: ILuaGuiElement(L, CLASS_LUAPROGRESBAR )							//конструктор
{}

int CLuaProgressBar::SetPosition( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel:SetPosition need int parameter");

	int position = lua_tointeger( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	_object->setPosition( position );

	return 1;
}

int CLuaProgressBar::SetImage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel:SetImage need string parameter");

	NrpText textureName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
		video::ITexture* ptrTexture = _nrpEngine.GetVideoDriver()->getTexture( textureName );
		_object->setImage( ptrTexture );
	}

	return 1;	
}

int CLuaProgressBar::SetFillImage( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaLabel:SetFillImage need string parameter");

	NrpText textureName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN
	{
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