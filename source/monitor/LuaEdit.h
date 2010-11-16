#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIEditBox;
	}
}

namespace nrp
{

class CLuaEdit : public ILuaGuiElement<irr::gui::IGUIEditBox>
{
public:
	static Luna<CLuaEdit>::RegType methods[];				//������ �������

	CLuaEdit(lua_State *L);		
																//������� �� ��� ������
	int SetOverrideColor( lua_State* L );
	int SetPasswordBox( lua_State* L );
	static const char* ClassName();
};

}//namespace nrp