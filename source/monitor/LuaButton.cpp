#include "StdAfx.h"
#include <IGUIButton.h>
#include <irrlicht.h>

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
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaButton::setImage need 5 parameter");

	std::string texturepath = lua_tostring( L, 6 );
	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	video::ITexture* txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath.c_str() ); //грузим текстуру в видеокарту
	
	IF_OBJECT_NOT_NULL_THEN	object_->setImage( txs, rectangle );								//размещаем текстуру в кнопке			
	
	return 1;
}

int CLuaButton::SetHoveredImage( lua_State *L )						//получает имя файла с текстурой, область из которой надо брать кнопку
																	//для текущего состояния
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaButton::setHoveredImage need 5 parameter");

	std::string texturepath = lua_tostring( L, 6 );
	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	video::ITexture* txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath.c_str() );

	IF_OBJECT_NOT_NULL_THEN	dynamic_cast< gui::CNrpButton* >( object_ )->setHoveredImage( txs, rectangle );

	return 1;
}

int CLuaButton::SetPressedImage( lua_State *L )						//получает имя файла с текстурой, область из которой надо брать кнопку
																	//для текущего состояния
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 6, 6, "Function CLuaButton::setPressedImage need 5 parameter"); //начинаем со второго параметра, ибо первым идет
																	//таблица с функциями	

	std::string texturepath = lua_tostring( L, 6 );
	core::recti rectangle;
	rectangle.UpperLeftCorner.X = lua_tointeger( L, 2 );
	rectangle.UpperLeftCorner.Y = lua_tointeger( L, 3 );
	rectangle.LowerRightCorner.X = lua_tointeger( L, 4 );
	rectangle.LowerRightCorner.Y = lua_tointeger( L, 5 );

	video::ITexture* txs = CNrpEngine::Instance().GetVideoDriver()->getTexture( texturepath.c_str() );

	IF_OBJECT_NOT_NULL_THEN	object_->setPressedImage( txs, rectangle );

	return 1;
}
	
int CLuaButton::SetAction( lua_State *L )									//устанавливает имя новой функции для этой кнопки	
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaButton::SetAction need string parameter");

	std::string funcName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN	dynamic_cast< gui::CNrpButton* >( object_ )->setOnClickAction( funcName.c_str() );

#ifdef _DEBUG
	char text[ MAX_PATH ];
	sprintf_s( text, MAX_PATH, "Object: %d  FuncName:%s\n", object_, funcName.c_str() );
	OutputDebugString( text );
#endif

	return 1;
}

}//namespace nrp