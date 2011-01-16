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
	DEFINE_PROPERTIES_AND_METHODS(CLuaMiniMap)
	static const char* ClassName();

	CLuaMiniMap(lua_State *L, bool);
	int SetTexture( lua_State *L );
	int GetAngleOffset( lua_State *L );
	int SetOnRotateAction( lua_State *L );
};

}//namespace nrp