/********************************************************************
created:	2009/11/19
created:	19:11:2009   16:40
filename: 	LuaButton.h
file path:	nerpa_sc
file base:	LuaButton
file ext:	h
author:		Dalerank

purpose:	Класс обертка для Lua, для работы с кнопками
			движка и изменения их параметров
*********************************************************************/
#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIButton;
	}
}

namespace nrp
{

CLASS_NAME CLASS_LUABUTTON( "CLuaButton" );

class CLuaButton : public ILuaGuiElement<irr::gui::IGUIButton>
{
public:
	static Luna<CLuaButton>::RegType methods[];				//методы обертки

	CLuaButton(lua_State *L);		
	int SetImage( lua_State *L );							//привязка к текстуре в обычном состоянии
	int SetHoveredImage( lua_State *L );					//привязка к текстуре в ховеред состоянии
	int SetPressedImage( lua_State *L );					//привязка к текстуре в нажатом состоянии
	int SetAction( lua_State *L );							//установка имени функции луа, которая будет вызвана при 
																//нажатии на эту кнопку
	static const char* ClassName() { return CLASS_LUABUTTON.c_str(); }
private:
	typedef enum { TI_IMAGE=0, TI_HOVER, TI_PRESSED } TYPE_IMAGE;
	int SetImage_( lua_State* L, std::string funcName, TYPE_IMAGE typeimg );
};

}//namespace nrp