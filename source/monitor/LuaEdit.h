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
	DEFINE_PROPERTIES_AND_METHODS(CLuaEdit)				//������ �������

	CLuaEdit(lua_State *L, bool);		
																//������� �� ��� ������
	int SetOverrideColor( lua_State* L );
	int SetPasswordBox( lua_State* L );
	int SetDrawBorder( lua_State* L );
	static const char* ClassName();
};

}//namespace nrp