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
	DEFINE_PROPERTIES_AND_METHODS(CLuaProgressBar)
	static const char* ClassName();

	CLuaProgressBar(lua_State *L, bool);		
	int SetPosition( lua_State* L );
	int SetImage( lua_State* L );
	int SetFillImage( lua_State* L );
	int SetAlphaUse( lua_State* L );
																//нажатии на эту кнопк
};

}//namespace nrp