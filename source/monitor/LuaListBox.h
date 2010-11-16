#pragma once

#include "ILuaListBox.h"

namespace irr
{
	namespace gui
	{
		class IGUIListBox;
	}
}

namespace nrp
{
class CLuaListBox : public ILuaListBox<irr::gui::IGUIListBox>
{
public:	
	static Luna<CLuaListBox>::RegType methods[];				//методы обертки
	static const char* ClassName();

	CLuaListBox(lua_State *L);		

	int AddItem( lua_State *L );
	int GetSelectedObject( lua_State* L );
};

}//namespace nrp