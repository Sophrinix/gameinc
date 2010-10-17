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
CLASS_NAME CLASS_COMPONENTLISTBOX( "CLuaComponentListBox" );

class CLuaComponentListBox : public ILuaGuiElement<irr::gui::CNrpComponentListbox>
{
public:
	static Luna<CLuaComponentListBox>::RegType methods[];				//методы обертки

	CLuaComponentListBox(lua_State *L);		
	
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedObject( lua_State* L );
	int SetSelected( lua_State *L );
	int SetItemHeigth( lua_State* L );
	int SetItemTextColor( lua_State* L );
	int SetFontFromSize( lua_State* L );
	int SetItemBgColor( lua_State* L );
	
	int Clear( lua_State* L );

	static const char* ClassName() { return CLASS_COMPONENTLISTBOX.c_str(); }
	virtual std::string ObjectName() { return CLASS_COMPONENTLISTBOX; }
};

}//namespace nrp