#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpProgressBar;
	}
}

namespace nrp
{

class CLuaProgressBar : public ILuaGuiElement<irr::gui::CNrpProgressBar>
{
public:
	static Luna<CLuaProgressBar>::RegType methods[];				//методы обертки

	CLuaProgressBar(lua_State *L);		
	int SetPosition( lua_State* L );
	int SetImage( lua_State* L );
	int SetFillImage( lua_State* L );
																//нажатии на эту кнопку
	static const char* StaticGetLuaName() { return "CLuaProgressBar"; }
};

}//namespace nrp