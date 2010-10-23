#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpWindow;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUAWINDOW( "CLuaWindow" );

class CLuaWindow : public ILuaGuiElement<irr::gui::CNrpWindow>
{
public:
	static Luna<CLuaWindow>::RegType methods[];

	CLuaWindow(lua_State *L);
	int GetCloseButton( lua_State *L );
	int SetDraggable( lua_State *L );
	int SetDrawBody( lua_State *L );
	int AddLuaFunction( lua_State* L );
	int RemoveLuaFunction( lua_State* L );
	int Draw( lua_State* L );

	static const char* ClassName() { return CLASS_LUAWINDOW.c_str(); }
};

}//namespace nrp;