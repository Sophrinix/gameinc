#pragma once
#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpScrollBar;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUASCROLLBAR( "CLuaScrollBar" );

class CLuaScrollBar : public ILuaGuiElement< irr::gui::CNrpScrollBar >
{
public:
	static Luna<CLuaScrollBar>::RegType methods[];

	CLuaScrollBar(lua_State *L);
	int SetTexture( lua_State *L );
	int SetAction( lua_State *L );
	int SetMax( lua_State* L );
	int SetMin( lua_State* L );
	int GetUpButton( lua_State* L );
	int GetDownButton( lua_State* L );
	int SetSliderTexture( lua_State* L );
	int GetPos( lua_State* L );

	static const char* ClassName() { return CLASS_LUASCROLLBAR.c_str(); }
	virtual std::string ObjectName() { return CLASS_LUASCROLLBAR; }
};

}//namespace nrp