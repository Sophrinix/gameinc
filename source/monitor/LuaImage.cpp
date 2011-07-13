#include "StdAfx.h"
#include <irrlicht.h>

#include "LuaImage.h"
#include "nrpEngine.h"
#include "LuaTexture.h"
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
    LUNA_AUTONAME_PROPERTY( CLuaImage, "rotate", PureFunction, SetRotate )
	LUNA_AUTONAME_PROPERTY( CLuaImage, "alphaChannel", PureFunction, SetUseAlphaChannel )
END_LUNA_PROPERTIES

CLuaImage::CLuaImage(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUAIMAGE, ex )							//конструктор
{}

int CLuaImage::SetImage( lua_State *L )								
{
	IF_OBJECT_NOT_NULL_THEN
	{
		video::ITexture* txs = NULL;
		if( lua_isstring( L, -1 ) )
		{
			NrpText pathToTexture = lua_tostring( L, -1 );
			txs = _nrpEngine.GetVideoDriver()->getTexture( pathToTexture );
		}
		else if( lua_istable( L, -1 ) || lua_isuserdata( L, -1 ) )
		{
			txs = _GetLuaObject< video::ITexture, CLuaTexture >( L, -1 );
		}
		_object->setImage( txs );
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

int CLuaImage::SetRotate( lua_State *L )
{
    assert( lua_isnumber( L, -1 ) );
    IF_OBJECT_NOT_NULL_THEN
    {
        float rotate = lua_tonumber( L, -1 );
        _object->setRotate( 180 + rotate );
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