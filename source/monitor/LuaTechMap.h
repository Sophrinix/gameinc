#pragma once

#include "ILuaGuiElement.h"

const std::string CLASS_LUATECHMAP( "CLuaTechMap" );

namespace irr
{
	namespace gui
	{
		class CNrpTechMap;
	}
}

namespace nrp
{

class CLuaTechMap : public ILuaGuiElement<irr::gui::CNrpTechMap>
{
public:
	static Luna<CLuaTechMap>::RegType methods[];				//������ �������

	CLuaTechMap(lua_State *L);									
	int SetImage( lua_State *L );							//�������� � �������� � ������� ���������
	int SetAction( lua_State *L );							//��������� ����� ������� ���, ������� ����� ������� ��� 
	int AddTechnology( lua_State* L );

	static const char* StaticGetLuaName() { return CLASS_LUATECHMAP.c_str(); }
};

}//namespace nrp