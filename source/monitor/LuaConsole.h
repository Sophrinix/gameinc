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
CLASS_NAME CLASS_LUACONSOLE( "CLuaConsole" );

class CLuaConsole : public ILuaGuiElement<irr::gui::CNrpConsole>
{
public:
	static Luna<CLuaConsole>::RegType methods[];

	CLuaConsole(lua_State *L);
	int ToggleVisible( lua_State *L );
	int Draw( lua_State* L );

	static const char* ClassName() { return CLASS_LUACONSOLE.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUACONSOLE; }
};

}//namespace nrp