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
	DEFINE_PROPERTIES_AND_METHODS(CLuaListBox)
	static const char* ClassName();

	CLuaListBox(lua_State *L, bool);		

	int AddItem( lua_State *L );
	int GetSelectedObject( lua_State* L );
};

}//namespace nrp