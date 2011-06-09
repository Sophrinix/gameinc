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
	DEFINE_PROPERTIES_AND_METHODS(CLuaConsole)

	CLuaConsole(lua_State *L, bool);
	int ToggleVisible( lua_State *L );
	int Bind( lua_State* L );
	int Unbind( lua_State* L );

	int Draw( lua_State* L );

	static const char* ClassName();
};

}//namespace nrp