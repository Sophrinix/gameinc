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
CLASS_NAME CLASS_LUATECHMAP( "CLuaTechMap" );

class CLuaTechMap : public ILuaGuiElement<irr::gui::CNrpTechMap>
{
public:
	static Luna<CLuaTechMap>::RegType methods[];				//������ �������

	CLuaTechMap(lua_State *L);									
	int SetImage( lua_State *L );							//�������� � �������� � ������� ���������
	int SetAction( lua_State *L );							//��������� ����� ������� ���, ������� ����� ������� ��� 
	int AddLuaFunction( lua_State* L );
	int AddTechnology( lua_State* L );
	int GetSelectedObjectType( lua_State* L );
	int GetSelectedObject( lua_State* L );
	int GetSelectedObjectName( lua_State* L );
	int SetDrawBack( lua_State* L );

	static const char* ClassName() { return CLASS_LUATECHMAP.c_str(); }
};

}//namespace nrp