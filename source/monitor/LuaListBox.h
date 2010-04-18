#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIListBox;
	}
}

namespace nrp
{

class CLuaListBox : public ILuaGuiElement<irr::gui::IGUIListBox>
{
public:
	static Luna<CLuaListBox>::RegType methods[];				//методы обертки

	CLuaListBox(lua_State *L);		
	
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedObject( lua_State* L );
	int SetSelected( lua_State *L );
	int Clear( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaListBox"; }
};

}//namespace nrp