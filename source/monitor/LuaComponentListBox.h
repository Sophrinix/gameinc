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
CLASS_NAME CLASS_COMPONENTLISTBOX( "CLuaComponentListBox" );

class CLuaComponentListBox : public ILuaListBox<irr::gui::CNrpComponentListbox>
{
public:
	static Luna<CLuaComponentListBox>::RegType methods[];				//методы обертки

	CLuaComponentListBox(lua_State *L);		
	
	int AddItem( lua_State *L );
	int GetSelectedObject( lua_State* L );

	static const char* ClassName() { return CLASS_COMPONENTLISTBOX.c_str(); }
};

}//namespace nrp