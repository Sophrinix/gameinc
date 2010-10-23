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

CLASS_NAME CLASS_LUALABEL( "CLuaLabel" );

class CLuaLabel : public ILuaGuiElement<irr::gui::IGUIStaticText>
{
public:
	static Luna<CLuaLabel>::RegType methods[];				//методы обертки

	CLuaLabel(lua_State *L);		
																//нажатии на эту кнопку
	static const char* ClassName() { return CLASS_LUALABEL.c_str(); }

	int SetOverrideColor( lua_State* L );
	int SetTextAlignment( lua_State* L );
	int SetOverrideFont( lua_State* L );
};

}//namespace nrp