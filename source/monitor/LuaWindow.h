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
	int AddLuaFunction( lua_State* L );
	int RemoveLuaFunction( lua_State* L );
	int Draw( lua_State* L );

	static const char* ClassName();
};

}//namespace nrp;