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
	DEFINE_PROPERTIES_AND_METHODS( CLuaComponentListBox )				//методы обертки

	CLuaComponentListBox(lua_State *L, bool);		
	
	int AddItem( lua_State *L );
	int GetSelectedObject( lua_State* L );
	int SetLmbDblClick( lua_State* L );
	static const char* ClassName();
};

}//namespace nrp