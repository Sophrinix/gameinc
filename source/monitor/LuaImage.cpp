#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaImage.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAIMAGE("CLuaImage");

Luna< CLuaImage >::RegType CLuaImage::methods[] =			//реализуемыe методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaImage ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaImage, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaImage, SetScaleImage ),
	LUNA_AUTONAME_FUNCTION( CLuaImage, SetUseAlphaChannel ),
	{0,0}
};

CLuaImage::CLuaImage(lua_State *L)	: ILuaGuiElement(L, CLASS_LUAIMAGE )							//конструктор
{}

int CLuaImage::SetImage( lua_State *L )								
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaImage::SetImage need ITexture* parameter");

	const char* pathToTexture = lua_tostring( L, 2 );
	assert( pathToTexture != NULL );

	IF_OBJECT_NOT_NULL_THEN
	{
		if( pathToTexture )
			object_->setImage( CNrpEngine::Instance().GetVideoDriver()->getTexture( pathToTexture ) );
	}

	return 1;
}

int CLuaImage::SetScaleImage( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaImage::SetScaleImage need bool parameter");

	bool scaleImage = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	object_->setScaleImage( scaleImage );

	return 1;
}

int CLuaImage::SetUseAlphaChannel( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaImage::SetUseAlphaChannel need bool parameter");

	bool useAlpha = lua_toboolean( L, 2 ) > 0;

	IF_OBJECT_NOT_NULL_THEN	object_->setUseAlphaChannel( useAlpha );

	return 1;
}

const char* CLuaImage::ClassName()
{
	return ( CLASS_LUAIMAGE );
}
}//namespace nrp