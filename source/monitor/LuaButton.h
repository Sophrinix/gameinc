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
		class IGUIButton;
	}
}

namespace nrp
{
class CLuaButton : public ILuaGuiElement<irr::gui::IGUIButton>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaButton)				//������ �������

	CLuaButton(lua_State *L, bool);		
	int SetImage( lua_State *L );							//�������� � �������� � ������� ���������
	int SetHoveredImage( lua_State *L );					//�������� � �������� � ������� ���������
	int SetPressedImage( lua_State *L );					//�������� � �������� � ������� ���������
	int SetAction( lua_State *L );							//��������� ����� ������� ���, ������� ����� ������� ���
	int SetPushButton( lua_State* L );
	int SetTooltip( lua_State* L );
	int SetPressed( lua_State* L );
	static const char* ClassName();
																//������� �� ��� ������
private:
	typedef enum { TI_IMAGE=0, TI_HOVER, TI_PRESSED } TYPE_IMAGE;
	int SetImage_( lua_State* L, const NrpText& funcName, TYPE_IMAGE typeimg );
};

}//namespace nrp