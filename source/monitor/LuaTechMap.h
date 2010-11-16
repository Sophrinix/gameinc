#pragma once

#include "ILuaGuiElement.h"

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
	static const char* ClassName();

	CLuaTechMap(lua_State *L);									
	int SetImage( lua_State *L );							//�������� � �������� � ������� ���������
	int SetAction( lua_State *L );							//��������� ����� ������� ���, ������� ����� ������� ��� 
	int AddLuaFunction( lua_State* L );
	int AddTechnology( lua_State* L );
	int GetSelectedObjectType( lua_State* L );
	int GetSelectedObject( lua_State* L );
	int GetSelectedObjectName( lua_State* L );
	int SetDrawBack( lua_State* L );
};

}//namespace nrp