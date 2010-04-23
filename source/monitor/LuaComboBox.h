/********************************************************************
created:	2009/11/19
created:	19:11:2009   16:40
filename: 	LuaButton.h
file path:	nerpa_sc
file base:	LuaButton
file ext:	h
author:		Dalerank

purpose:	����� ������� ��� Lua, ��� ������ � ��������
			������ � ��������� �� ����������
*********************************************************************/
#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIComboBox;
	}
}

namespace nrp
{

class CLuaComboBox : public ILuaGuiElement<irr::gui::IGUIComboBox>
{
public:
	static Luna<CLuaComboBox>::RegType methods[];				//������ �������

	CLuaComboBox(lua_State *L);		
	
	int SetImage( lua_State *L );							//�������� � �������� � ������� ���������
	int SetAction( lua_State *L );							//��������� ����� ������� ���, ������� ����� ������� ��� 
	int Clear( lua_State* L );
																//������� �� ��� ������
	int AddItem( lua_State *L );
	int GetSelected( lua_State *L );
	int GetSelectedObject( lua_State *L );
	int SetSelected( lua_State *L );

	static const char* StaticGetLuaName() { return "CLuaComboBox"; }
};

}//namespace nrp