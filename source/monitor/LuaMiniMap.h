#pragma once
#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpMiniMap;
	}
}

namespace nrp
{

class CLuaMiniMap : public ILuaGuiElement< irr::gui::CNrpMiniMap >
{
public:
	static Luna<CLuaMiniMap>::RegType methods[];
	static const char* ClassName();

	CLuaMiniMap(lua_State *L);
	int SetTexture( lua_State *L );
	int GetAngleOffset( lua_State *L );
	int SetOnRotateAction( lua_State *L );
};

}//namespace nrp