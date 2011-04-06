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
class CLuaButton : public ILuaGuiElement<irr::gui::IGUIButton>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaButton)				//методы обертки

	CLuaButton(lua_State *L, bool);		
	int SetImage( lua_State *L );							//привязка к текстуре в обычном состоянии
	int SetHoveredImage( lua_State *L );					//привязка к текстуре в ховеред состоянии
	int SetPressedImage( lua_State *L );					//привязка к текстуре в нажатом состоянии
	int SetAction( lua_State *L );							//установка имени функции луа, которая будет вызвана при
	int SetPushButton( lua_State* L );
	int SetTooltip( lua_State* L );
	int SetPressed( lua_State* L );
	static const char* ClassName();
																//нажатии на эту кнопку
private:
	typedef enum { TI_IMAGE=0, TI_HOVER, TI_PRESSED } TYPE_IMAGE;
	int SetImage_( lua_State* L, const NrpText& funcName, TYPE_IMAGE typeimg );
};

}//namespace nrp