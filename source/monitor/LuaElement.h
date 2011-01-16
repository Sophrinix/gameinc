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
	DEFINE_PROPERTIES_AND_METHODS(CLuaElement)

	CLuaElement(lua_State *L, bool );	
	static const char* ClassName();
};

}//namespace nrp