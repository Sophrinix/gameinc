#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIElement;
	}
}

namespace nrp
{

class CLuaElement : public ILuaGuiElement<irr::gui::IGUIElement>
{
public:
	static Luna<CLuaElement>::RegType methods[];				//методы обертки

	CLuaElement(lua_State *L);	
	int RemoveChilds( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaElement"; }
};

}//namespace nrp