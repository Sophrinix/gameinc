#pragma once

#include "ILuaListBox.h"

namespace irr
{
	namespace gui
	{
		class CNrpComponentListbox;
	}
}

namespace nrp
{
class CLuaComponentListBox : public ILuaListBox<irr::gui::CNrpComponentListbox>
{
public:
	static Luna<CLuaComponentListBox>::RegType methods[];				//методы обертки

	CLuaComponentListBox(lua_State *L);		
	
	int AddItem( lua_State *L );
	int GetSelectedObject( lua_State* L );
	static const char* ClassName();
};

}//namespace nrp