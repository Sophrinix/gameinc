#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIStaticText;
	}
}

namespace nrp
{

class CLuaLabel : public ILuaGuiElement<irr::gui::IGUIStaticText>
{
public:
	static Luna<CLuaLabel>::RegType methods[];				//методы обертки

	CLuaLabel(lua_State *L);		
																//нажатии на эту кнопку
	static const char* StaticGetLuaName() { return "CLuaLabel"; }
	int SetOverrideColor( lua_State* L );
	int SetTextAlignment( lua_State* L );
	int SetOverrideFont( lua_State* L );
};

}//namespace nrp