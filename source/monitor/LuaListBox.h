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
CLASS_NAME CLASS_LUALISTBOX( "CLuaListBox" );

class CLuaListBox : public ILuaGuiElement<irr::gui::IGUIListBox>
{
public:
	static Luna<CLuaListBox>::RegType methods[];				//методы обертки

	CLuaListBox(lua_State *L);		
	
	int AddItem( lua_State *L );
	int SetItemBgColor( lua_State* L );
	int GetSelected( lua_State *L );
	int GetSelectedObject( lua_State* L );
	int SetSelected( lua_State *L );
	int Clear( lua_State* L );
	int SetFontFromSize( lua_State* L );
	int SetItemTextColor( lua_State* L );

	static const char* ClassName() { return CLASS_LUALISTBOX.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUALISTBOX; }
};

}//namespace nrp