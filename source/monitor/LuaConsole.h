#pragma once
#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpConsole;
	}
}

namespace nrp
{

class CLuaConsole : public ILuaGuiElement<irr::gui::CNrpConsole>
{
public:
	static Luna<CLuaConsole>::RegType methods[];

	CLuaConsole(lua_State *L);
	int ToggleVisible( lua_State *L );

	static const char* StaticGetLuaName() { return "CLuaConsole"; }
};

}//namespace nrp