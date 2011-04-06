#include "StdAfx.h"
#include <IGUIButton.h>
#include <irrlicht.h>
#include <assert.h>

#include "LuaButton.h"
#include "nrpEngine.h"
#include "nrpButton.h"
#include "nrpGUIEnvironment.h"
#include "nrpButton.h"

using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUABUTTON( "CLuaButton" );

BEGIN_LUNA_METHODS(CLuaButton)
	LUNA_ILUAGUIELEMENT_HEADER( CLuaButton )
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetImage )
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetHoveredImage )
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetPressedImage )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaButton)
	LUNA_ILUAGUIELEMENT_PROPERTIES( CLuaButton )
	LUNA_AUTONAME_PROPERTY( CLuaButton, "action", PureFunction, SetAction )
	LUNA_AUTONAME_PROPERTY( CLuaButton, "pushButton", PureFunction, SetPushButton )
	LUNA_AUTONAME_PROPERTY( CLuaButton, "pressed", PureFunction, SetPressed )
	LUNA_AUTONAME_PROPERTY( CLuaButton, "tooltip", PureFunction, SetTooltip )
END_LUNA_PROPERTIES


const char* CLuaButton::ClassName()
{
	return 	( CLASS_LUABUTTON );
}

CLuaButton::CLuaButton(lua_State *L, bool ex)	: ILuaGuiElement(L, CLASS_LUABUTTON, ex )							//конструктор
{}

int CLuaButton::SetPushButton( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		bool pushBtn = lua_toboolean( L, -1 ) > 0;
		_object->setIsPushButton( pushBtn );
	}

	return 0;
}

int CLuaButton::SetPressed( lua_State* L )
{
	IF_OBJECT_NOT_NULL_THEN 
	{
		bool pushed = lua_toboolean( L, -1 ) > 0;
		_object->setPressed( pushed );
	}

	return 0;
}

int CLuaButton::SetImage( lua_State *L )							//получает имя файла с текстурой, область из которой надо брать кнопку
																	//для текущего состояния
{
	return SetImage_( L, "SetHoveredImage", TI_IMAGE );
}

int CLuaButton::SetHoveredImage( lua_State *L )						//получает имя файла с текстурой, область из которой надо брать кнопку
																	//для текущего состояния
{
	return SetImage_( L, "SetHoveredImage", TI_HOVER );
}

int CLuaButton::SetPressedImage( lua_State *L )						//получает имя файла с текстурой, область из которой надо брать кнопку
																	//для текущего состояния
{
	return SetImage_( L, "SetPressedImage", TI_PRESSED );
}
	
int CLuaButton::SetAction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	IF_OBJECT_NOT_NULL_THEN
		dynamic_cast< gui::CNrpButton* >( _object )->setOnClickAction( _GetRef( L, -1 ) );

	return 1;
}

int CLuaButton::SetImage_( lua_State* L, const NrpText& funcName, TYPE_IMAGE typeimg )
{
	int argc = lua_gettop(L);
	//начинаем со второго параметра, ибо первым идет таблица с функциями	
	luaL_argcheck(L, argc == 6, 6, _ErrStr( funcName + " need string, rectangle parameter" ) ); 

	core::recti rectangle;
	NrpText texturepath = lua_tostring( L, 6 );

	video::ITexture* txs = _nrpEngine.GetVideoDriver()->getTexture( texturepath );

	rectangle = _ReadRect( L, 2, NULL );
	if( rectangle == core::recti( 0, 0, 0, 0) && ( txs != NULL ) )
		rectangle = core::recti( 0, 0, txs->getSize().Width, txs->getSize().Height );

	IF_OBJECT_NOT_NULL_THEN
		switch( typeimg ) 
		{
		case TI_IMAGE: _object->setImage( txs, rectangle ); break;
		case TI_HOVER: dynamic_cast< gui::CNrpButton* >( _object )->setHoveredImage( txs, rectangle ); break;
		case TI_PRESSED: _object->setPressedImage( txs, rectangle ); break;
		}

	return 1;
}

int CLuaButton::SetTooltip( lua_State* L )
{
	assert( lua_isstring( L, -1 ) );
	NrpText text( lua_tostring( L, -1 ) );
	IF_OBJECT_NOT_NULL_THEN	_object->setToolTipText( text.ToWide() );
	return 1;
}
}//namespace nrp