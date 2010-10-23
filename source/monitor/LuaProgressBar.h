#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class CNrpProgressBar;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUAPROGRESBAR( "CLuaProgressBar" );

class CLuaProgressBar : public ILuaGuiElement<irr::gui::CNrpProgressBar>
{
public:
	static Luna<CLuaProgressBar>::RegType methods[];				//������ �������

	CLuaProgressBar(lua_State *L);		
	int SetPosition( lua_State* L );
	int SetImage( lua_State* L );
	int SetFillImage( lua_State* L );
																//������� �� ��� ������
	static const char* ClassName() { return CLASS_LUAPROGRESBAR.c_str(); }
};

}//namespace nrp