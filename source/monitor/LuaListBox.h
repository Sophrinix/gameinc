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
CLASS_NAME CLASS_LUALISTBOX( "CLuaListBox" );

class CLuaListBox : public ILuaListBox<irr::gui::IGUIListBox>
{
public:	
	static Luna<CLuaListBox>::RegType methods[];				//методы обертки
	CLuaListBox(lua_State *L);		

	int AddItem( lua_State *L );
	int GetSelectedObject( lua_State* L );

	static const char* ClassName() { return CLASS_LUALISTBOX.c_str(); }
};

}//namespace nrp