#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaImage.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUAIMAGE("CLuaImage");

BEGIN_LUNA_METHODS(CLuaImage)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaImage )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaImage)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaImage )
	LUNA_AUTONAME_PROPERTY( CLuaImage, "texture", PureFunction, SetImage )
	LUNA_AUTONAME_PROPERTY( CLuaImage, "scale", PureFunction, SetScaleImage )
	LUNA_AUTONAME_PROPERTY( CLuaImage, "alphaChannel", PureFunction, SetUseAlphaChannel )
END_LUNA_PROPERTIES

CLuaImage::CLuaImage(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAIMAGE, ex )							//конструктор
{}

int CLuaImage::SetImage( lua_State *L )								
{
	assert( lua_isstring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText pathToTexture = lua_tostring( L, -1 );
		_object->setImage( _nrpEngine.GetVideoDriver()->getTexture( pathToTexture ) );
	}

	return 0;
}

int CLuaImage::SetScaleImage( lua_State *L )
{
	assert( lua_isboolean( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN
	{
		bool scaleImage = lua_toboolean( L, -1 ) > 0;
		_object->setScaleImage( scaleImage );
	}

	return 0;
}

int CLuaImage::SetUseAlphaChannel( lua_State* L )
{
	assert( lua_isboolean( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	
	{
		bool useAlpha = lua_toboolean( L, 2 ) > 0;
		_object->setUseAlphaChannel( useAlpha );
	}

	return 0;
}

const char* CLuaImage::ClassName()
{
	return ( CLASS_LUAIMAGE );
}
}//namespace nrp