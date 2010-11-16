#pragma once
#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUITab;
		class IGUIFont;
	}
}

namespace nrp
{

class CLuaTab : public ILuaGuiElement<irr::gui::IGUITab>
{
public:
	static Luna<CLuaTab>::RegType methods[];
	static const char* ClassName();

	CLuaTab(lua_State *L);
	int SetFontFromSize( lua_State *L );
	int SetTextColor( lua_State *L );
};

}//namespace nrp