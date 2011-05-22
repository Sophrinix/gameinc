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

class CLuaWindow : public ILuaGuiElement<irr::gui::CNrpWindow>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaWindow)

	CLuaWindow(lua_State *L, bool exist );

	int GetCloseButton( lua_State *L );
	int SetDraggable( lua_State *L );
	int SetDrawBody( lua_State *L );
	int Bind( lua_State* L );
	int Unbind( lua_State* L );
	int Draw( lua_State* L );
	int SetOnRemove( lua_State* L );
	int SetOnLmbClick( lua_State* L );
	int SetOnKeyEvent( lua_State* L );

	int SetTexture( lua_State* L );
	int GetTexture( lua_State* L );

	static const char* ClassName();
};

}//namespace nrp;