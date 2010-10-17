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
CLASS_NAME CLASS_LUATAB( "CLuaTab" );

class CLuaTab : public ILuaGuiElement<irr::gui::IGUITab>
{
public:
	static Luna<CLuaTab>::RegType methods[];

	CLuaTab(lua_State *L);
	int SetFontFromSize( lua_State *L );
	int SetTextColor( lua_State *L );

	static const char* ClassName() { return CLASS_LUATAB.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUATAB; }
};

}//namespace nrp