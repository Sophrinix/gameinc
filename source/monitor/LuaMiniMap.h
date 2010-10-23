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
CLASS_NAME CLASS_LUAMINIMAP( "CLuaMiniMap" );

class CLuaMiniMap : public ILuaGuiElement< irr::gui::CNrpMiniMap >
{
public:
	static Luna<CLuaMiniMap>::RegType methods[];

	CLuaMiniMap(lua_State *L);
	int SetTexture( lua_State *L );
	int GetAngleOffset( lua_State *L );
	int SetOnRotateAction( lua_State *L );

	static const char* ClassName() { return CLASS_LUAMINIMAP.c_str(); }
};

}//namespace nrp