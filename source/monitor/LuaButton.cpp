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

Luna< CLuaButton >::RegType CLuaButton::methods[] =			//реализуемы методы
{
	LUNA_ILUAGUIELEMENT_HEADER( CLuaButton ),
	/*   */
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetImage ),
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetHoveredImage ),
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetPressedImage ),
	LUNA_AUTONAME_FUNCTION( CLuaButton, SetAction ),
	{0,0}
};

CLuaButton::CLuaButton(lua_State *L)	: ILuaGuiElement(L, "CLuaButton")							//конструктор
{}

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
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::SetAction need string parameter");

	const char* funcName = lua_tostring( L, 2 );
	assert( funcName != NULL );

	IF_OBJECT_NOT_NULL_THEN	dynamic_cast< gui::CNrpButton* >( object_ )->setOnClickAction( funcName );

#ifdef _DEBUG
	char text[ MAX_PATH ];
	sprintf_s( text, MAX_PATH, "Object: %d  FuncName:%s\n", object_, funcName );
	OutputDebugString( text );
#endif

	return 1;
}

int CLuaButton::SetImage_( lua_State* L, std::string funcName, TYPE_IMAGE typeimg )
{
	int argc = lua_gettop(L);
	//начинаем со второго параметра, ибо первым идет таблица с функциями	
	luaL_argcheck(L, argc == 6, 6, ("Function CLuaButton:" + funcName + " need string, rectangle parameter").c_str() ); 

	core::recti rectangle;
	const char* texturepath = lua_tostring( L, 6 );
	assert( texturepath != NULL );
	video::ITexture* txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath );

	rectangle = ReadRect_( L, 2 );
	if( rectangle == core::recti( 0, 0, 0, 0) && ( txs != NULL ) )
		rectangle = core::recti( 0, 0, txs->getSize().Width, txs->getSize().Height );

	IF_OBJECT_NOT_NULL_THEN
		switch( typeimg ) 
		{
		case TI_IMAGE: object_->setImage( txs, rectangle ); break;
		case TI_HOVER: dynamic_cast< gui::CNrpButton* >( object_ )->setHoveredImage( txs, rectangle ); break;
		case TI_PRESSED: object_->setPressedImage( txs, rectangle ); break;
		}

	return 1;
}
}//namespace nrp