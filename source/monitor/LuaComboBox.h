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
		class IGUIComboBox;
	}
}

namespace nrp
{

class CLuaComboBox : public ILuaGuiElement<irr::gui::IGUIComboBox>
{
public:
	static Luna<CLuaComboBox>::RegType methods[];				//методы обертки

	CLuaComboBox(lua_State *L);		
	
	int SetImage( lua_State *L );							//привязка к текстуре в обычном состоянии
	int SetAction( lua_State *L );							//установка имени функции луа, которая будет вызвана при 
	int Clear( lua_State* L );
																//нажатии на эту кнопку
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedObject( lua_State *L );
	int SetSelected( lua_State *L );

	static const char* StaticGetLuaName() { return "CLuaComboBox"; }
};

}//namespace nrp