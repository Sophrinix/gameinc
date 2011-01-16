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
	DEFINE_PROPERTIES_AND_METHODS(CLuaTab)
	static const char* ClassName();

	CLuaTab(lua_State *L, bool);
	int SetFontFromSize( lua_State *L );
	int SetTextColor( lua_State *L );
};

}//namespace nrp