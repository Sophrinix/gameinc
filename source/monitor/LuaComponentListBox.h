#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpComponentListbox;
	}
}

namespace nrp
{

class CLuaComponentListBox : public ILuaGuiElement<irr::gui::CNrpComponentListbox>
{
public:
	static Luna<CLuaComponentListBox>::RegType methods[];				//методы обертки

	CLuaComponentListBox(lua_State *L);		
	
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedObject( lua_State* L );
	int SetSelected( lua_State *L );
	int Clear( lua_State* L );

	static const char* StaticGetLuaName() { return "CLuaComponentListBox"; }
};

}//namespace nrp