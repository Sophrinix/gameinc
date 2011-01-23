#pragma once

#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpGuiFlick;
	}
}

namespace nrp
{

class CLuaFlick : public ILuaGuiElement<irr::gui::CNrpGuiFlick>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaFlick)

	CLuaFlick(lua_State *L, bool );	
	int Clear( lua_State* L );
	static const char* ClassName();
};

}//namespace nrp