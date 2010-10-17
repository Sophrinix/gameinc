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
CLASS_NAME CLASS_LUAELEMENT( "CLuaElement" );

class CLuaElement : public ILuaGuiElement<irr::gui::IGUIElement>
{
public:
	static Luna<CLuaElement>::RegType methods[];				//методы обертки

	CLuaElement(lua_State *L);	

	static const char* ClassName() { return CLASS_LUAELEMENT.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUAELEMENT; }
};

}//namespace nrp